#include <cassert>
#include <cstring>
#include <cstdint>
#include <cstdio>

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>

constexpr auto size_width =640;
constexpr auto size_height = 480;

int32_t buf[size_height*size_height];

auto put_pixel(int x, int y, uint32_t color) -> void {
    buf[(y * size_width) + x] = color;
}

auto fill_circle(int x, int y, int r, uint32_t c) -> void {
    for (int dy = -r; dy < r; dy++) {
        for (int dx = -r; dx < r; dx++) {
            if (dx * dx + dy * dy <= r * r)
                put_pixel(x + dx, y + dy, c);
        }
    }
}

int main()
{
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_gcontext_t graphics_context;
    xcb_window_t window_xcb;
    xcb_image_t* xcb_image;
    auto format = XCB_IMAGE_FORMAT_Z_PIXMAP;
    uint8_t depth;
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2];

    connection = xcb_connect(NULL, NULL);
    assert(connection);
    screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
    depth = screen->root_depth;
    values[0] = screen->black_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS;

    window_xcb = xcb_generate_id(connection);
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window_xcb, screen->root, 0, 0, size_width,
                      size_height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, values);
    xcb_map_window(connection, window_xcb);
    xcb_flush(connection);


    graphics_context = xcb_generate_id(connection);
    xcb_create_gc(connection, graphics_context, window_xcb, 0, nullptr);
    xcb_flush(connection);
    xcb_image =
        xcb_image_create_native(connection, size_width, size_height,
                                format, depth, (uint8_t *)buf,
                                size_width * size_height * 4,
                                (uint8_t *)buf);

    printf("Maximum request size is 4 * %d bytes while we will be trying to send 4 * %d bytes of data (plus request overhead)\n", xcb_get_maximum_request_length(connection), size_width * size_height);
    xcb_flush(connection);
    memset(buf, size_width*size_height*4, 0xff);
    fill_circle(50, 50, 20, 0xff0000);
    auto maximum_request_length = xcb_get_maximum_request_length(connection);

    {
        xcb_generic_event_t *event;

        while (true) {
            event = xcb_wait_for_event(connection);
            if (event == nullptr) {
                printf("Connection broke with status %d\n", xcb_connection_has_error(connection));
                break;
            }
            switch (event->response_type & 0x7f) {
            case XCB_EXPOSE: {
                xcb_expose_event_t *ev = (xcb_expose_event_t *)event;

                auto depth = screen->root_depth;
                xcb_pixmap_t pixmap = xcb_generate_id(connection);
                xcb_create_pixmap(connection, depth, pixmap, ev->window,
                                  size_width, size_height);

                auto stride= size_width;
                const uint32_t req_size = 18;
                uint32_t length = size_height * stride;
                uint32_t len = (req_size + length * 4);


                // https://stackoverflow.com/a/76188923/78712https://stackoverflow.com/a/76188923/78712
                if (len < maximum_request_length) {
                    xcb_image_put(connection, window_xcb, graphics_context, xcb_image, 0, 0, 0);
                } else {
                    auto height = size_height;
                    auto width = size_width;
                    auto dst_y = 0;
                    auto data = (uint8_t *) buf;
                    int rows = (maximum_request_length - req_size - 4) / stride;
                    if (rows > 0) {
                        do {
                        if (rows > height)
                            rows = height;

                        length = rows * stride;

                        xcb_put_image (connection, XCB_IMAGE_FORMAT_Z_PIXMAP,
                                   0, graphics_context, width, rows, 0, dst_y, 0, depth, length,  data);

                        height -= rows;
                        dst_y += rows;
                        data =  data + length;
                        } while (height);
                    }
                }
                xcb_flush(connection);
            } break;
            case XCB_DESTROY_NOTIFY:
                break;
            default:
                //            xcb_free_event(event);
                break;
            }
        }
    }

    return 0;
}
