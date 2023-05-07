#include "platform-xcb.h"
#include "theme.h"
#include "widget.h"

#include <memory.h>
#include <spdlog/spdlog.h>
#include <xcb/xcb_image.h>

struct PlatformWindowXCB : PlatformWindow {
    xcb_window_t xcb_window;
    xcb_image_t *xcb_image;
    xcb_gcontext_t gc;

    virtual ~PlatformWindowXCB() override {
        // XFree(x11_image);
    }
};

#define XKB_X11_MIN_MAJOR_XKB_VERSION 1
#define XKB_X11_MIN_MINOR_XKB_VERSION 0
#define XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS 0

auto PlatformXCB::init() -> void {
    connection = xcb_connect(NULL, NULL);
    assert(connection);

    screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

#if 0
    default_theme = std::make_shared<ThemePlasma>();
#else
    default_theme = std::make_shared<ThemeRedmond>();
//    default_theme = std::make_shared<ThemeVision>();
#endif
    spdlog::info("PlatformXCB initialized");
}

auto PlatformXCB::done() -> void { xcb_disconnect(connection); }

auto PlatformXCB::open_window(int x, int y, int width, int height, const std::string title)
    -> std::shared_ptr<PlatformWindow> {
    auto no_border = 0;

    xcb_gcontext_t graphics_context;
    xcb_window_t window_xcb;
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2] = {screen->black_pixel,
                          XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS};

    window_xcb = xcb_generate_id(connection);
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window_xcb, screen->root, 0, 0, width,
                      height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, values);

    graphics_context = xcb_generate_id(connection);
    xcb_create_gc(connection, graphics_context, window_xcb, mask, values);

    auto window = std::make_shared<PlatformWindowXCB>();
    window->xcb_window = window_xcb;
    window->gc = graphics_context;
    window->title = title;
    window->content.resize(width, height);
    window->theme = default_theme;
    window->platform = this;
    window->content.resize(width, height);

    auto format = XCB_IMAGE_FORMAT_Z_PIXMAP;
    auto depth = screen->root_depth;
    window->xcb_image =
        xcb_image_create_native(connection, window->content.size.width, window->content.size.height,
                                format, depth, (uint8_t *)window->content.buf,
                                window->content.size.width * window->content.size.height * 4,
                                (uint8_t *)window->content.buf);
    windows[window->xcb_window] = window;
    return window;
}

auto PlatformXCB::show_window(std::shared_ptr<PlatformWindow> w) -> void {
    auto window = std::dynamic_pointer_cast<PlatformWindowXCB>(w);
    xcb_map_window(connection, window->xcb_window);
    xcb_flush(connection);
    // Set the window's properties.
    //    xcb_change_window_attributes(connection, window, XCB_CW_TITLE,
    //                                 &xcb_atom_t{XCB_ATOM_STRING, "My Window"});
    //    xcb_change_window_attributes(connection, window, XCB_CW_BACK_PIXEL,
    //                                 &xcb_pixel_t{BlackPixel(connection, 0)});
}

auto PlatformXCB::invalidate(PlatformWindow &w) -> void {
    auto window = static_cast<PlatformWindowXCB *>(&w);
    //    invalidate_impl(connection, *window);

    //    auto pixmap = window.xcb_image;
    //    window.draw();
    //    xcb_copy_area(connection, pixmap, window.xcb_window, window.gc, 0, 0, 0, 0,
    //                  window.content.size.width, window.content.size.height);
    //    xcb_flush(connection);
}
#include <xcb/xcb_image.h>
auto PlatformXCB::main_loop() -> void {

    xcb_generic_event_t *event;

    while (!this->exit_loop && (event = xcb_wait_for_event(connection))) {
        std::shared_ptr<PlatformWindowXCB> window;
        spdlog::debug("Sending message {} to \"{}\"", event->response_type, "");

        switch (event->response_type) {
        case XCB_EXPOSE: {
            spdlog::info("EXPOSE");
            xcb_expose_event_t *ev = (xcb_expose_event_t *)event;
            if (windows.find(ev->window) == windows.end()) {
                spdlog::critical("Window =({}) no longer alive, ignoring event {}", ev->window,
                                 ev->response_type);
                continue;
            }
            window = windows.at(ev->window);
            window->needs_redraw = true;
            //            invalidate_impl(connection, *window.get());
        } break;
        case XCB_DESTROY_NOTIFY:
            break;
        default:
            //            xcb_free_event(event);
            break;
        }

        if (window && window->needs_redraw) {
            window->draw();
            auto depth = screen->root_depth;
            xcb_pixmap_t pixmap = xcb_generate_id(connection);
            xcb_create_pixmap(connection, depth, pixmap, window->xcb_window,
                              window->content.size.width, window->content.size.height);
            xcb_image_put(connection, pixmap
                          , window->gc, window->xcb_image, 0, 0, 0);
            xcb_copy_area(connection, pixmap, window->xcb_window, window->gc, 0, 0, 0, 0,
                          window->content.size.width, window->content.size.height);
            xcb_flush(connection);
        }

        if (this->close_on_last_window && this->windows.size() == 0) {
            this->exit_loop = true;
            spdlog::info("No more windows - closing event loop");
        }
    }
}
