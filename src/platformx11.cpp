#include <iostream>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

#include "platformx11.h"
#include "spdlog/spdlog.h"

// lookup table to convert X11 keyboard events to "Psuedo ASCII"
// TODO - make constants for our own keyboard events
static int X11_KEYCODES[124] = {
    XK_BackSpace, 8,
    XK_Delete, 127,
    XK_Down, 18,
    XK_End, 5,
    XK_Escape, 27,
    XK_Home, 2,
    XK_Insert, 26,
    XK_Left, 20,
    XK_Page_Down, 4,
    XK_Page_Up, 3,
    XK_Return, 10,
    XK_Right, 19,
    XK_Tab, 9,
    XK_Up, 17,
    XK_apostrophe, 39,
    XK_backslash, 92,
    XK_bracketleft, 91,
    XK_bracketright, 93,
    XK_comma, 44,
    XK_equal, 61,
    XK_grave, 96,
    XK_minus, 45,
    XK_period, 46,
    XK_semicolon, 59,
    XK_slash, 47,
    XK_space, 32,
    XK_a, 65,
    XK_b, 66,
    XK_c, 67,
    XK_d, 68,
    XK_e, 69,
    XK_f, 70,
    XK_g, 71,
    XK_h, 72,
    XK_i, 73,
    XK_j, 74,
    XK_k, 75,
    XK_l, 76,
    XK_m, 77,
    XK_n, 78,
    XK_o, 79,
    XK_p, 80,
    XK_q, 81,
    XK_r, 82,
    XK_s, 83,
    XK_t, 84,
    XK_u, 85,
    XK_v, 86,
    XK_w, 87,
    XK_x, 88,
    XK_y, 89,
    XK_z, 90,
    XK_0, 48,
    XK_1, 49,
    XK_2, 50,
    XK_3, 51,
    XK_4, 52,
    XK_5, 53,
    XK_6, 54,
    XK_7, 55,
    XK_8, 56,
    XK_9, 57};

auto convert_x11_key_event(const XEvent &ev, Display *dpy) -> EventKeyboard
{
    auto event = EventKeyboard();
    auto k = XkbKeycodeToKeysym(dpy, ev.xkey.keycode, 0, 0);
    for (auto i = 0; i < 124; i += 2)
    {
        /* Map XKB KeySym to our custom key code value */
        if (X11_KEYCODES[i] == k)
        {
            event.key = X11_KEYCODES[i + 1];
            break;
        }
    }
    int m = ev.xkey.state;

    event.keydown = ev.type == KeyPress;
    event.modifiers = (!!(m & ControlMask)) | (!!(m & ShiftMask) << 1) |
                      (!!(m & Mod1Mask) << 2) | (!!(m & Mod4Mask) << 3);
    return event;
}

auto convert_x11_mouse_event(const XEvent &ev, Display *dpy) -> EventMouse
{
    auto event = EventMouse();
    switch (ev.type)
    {
    case MotionNotify:
        event.x = ev.xmotion.x;
        event.y = ev.xmotion.y;
        break;
    case ButtonPress:
        event.button = ev.xbutton.button;
        event.pressed = true;
        break;
    case ButtonRelease:
        event.button = ev.xbutton.button;
        event.pressed = false;
        break;
    }
    return event;
}

auto convert_x11_configure_event(const XEvent &ev) -> EventResize
{
    auto event = EventResize();
    event.size.width = ev.xconfigure.width;
    event.size.height = ev.xconfigure.height;
    event.position.x = ev.xconfigure.x;
    event.position.y = ev.xconfigure.y;
    return event;
}

struct PlatformWindowX11 : public PlatformWindow
{
    Window x11_window;
    XImage *x11_image = nullptr;

    // TODO - is a GContent a global variable, and not per window?
    GC gc;

    virtual ~PlatformWindowX11() override
    {
        XFree(x11_image);
        // XDestroyWindow(dpy, x11_window);
        // XFreeGC(dpy, gc);
    }
};

auto PlatformX11::init() -> void
{
    spdlog::set_level(spdlog::level::info);
    spdlog::info("PlatformX11 initialized");
    dpy = XOpenDisplay(NULL);
    screen = DefaultScreen(dpy);

    wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    if (wmDeleteMessage == None)
    {
        spdlog::error("Could not get redraw atom from WM");
        return;
    }
}

auto PlatformX11::done() -> void
{
    // TODO close all windows
    XCloseDisplay(dpy);
}

auto PlatformX11::open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow>
{
    auto window = std::make_shared<PlatformWindowX11>();
    window->title = title;
    window->x11_window = XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 0, 0, width,
                                             height, 0, BlackPixel(dpy, screen),
                                             WhitePixel(dpy, screen));
    window->gc = XCreateGC(dpy, window->x11_window, 0, 0);
    XSelectInput(dpy, window->x11_window,
                 ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
                     ButtonReleaseMask | PointerMotionMask);
    XStoreName(dpy, window->x11_window, title.c_str());
    XSetWindowBackgroundPixmap(dpy, window->x11_window, None);
    wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, window->x11_window, &wmDeleteMessage, 1);

    windows[window->x11_window] = window;
    return window;
}

auto PlatformX11::show_window(std::shared_ptr<PlatformWindow> w) -> void
{
    XMapWindow(dpy, window->x11_window);
    XSync(dpy, window->x11_window);

    window->content.resize(width, height);
    window->x11_image = XCreateImage(dpy, DefaultVisual(dpy, 0), 24, ZPixmap, 0,
                                     (char *)window->content.buf,
                                     window->content.size.width,
                                     window->content.size.height, 32, 0);
}

auto PlatformX11::main_loop() -> void
{
    XEvent ev;
    int pending;

    while (pending = XPending(dpy) || !this->exit_loop)
    {
        auto k = XNextEvent(dpy, &ev);
        std::shared_ptr<PlatformWindowX11> target_window;

        if (windows.find(ev.xany.window) == windows.end())
        {
            spdlog::critical("Window =({}) no longer alive, ignoring event {}", ev.xany.window, ev.type);
            continue;
        }
        target_window = windows.at(ev.xany.window);
        spdlog::debug("Sending message {} to \"{}\"", ev.type, target_window->title);

        switch (ev.type)
        {
        case Expose:
            target_window->needs_redraw = true;
            break;

        case ClientMessage:
            spdlog::debug("Atom {}", ev.xclient.data.l[0]);
            if (ev.xclient.data.l[0] == wmDeleteMessage)
            {
                if (target_window->can_close())
                {
                    XDestroyWindow(dpy, target_window->x11_window);
                }
            }
            break;

        case UnmapNotify:
            break;
        case DestroyNotify:
            windows.extract(ev.xany.window);
            break;

        case ButtonPress:
        case ButtonRelease:
        case MotionNotify:
            target_window->on_mouse(convert_x11_mouse_event(ev, this->dpy));
            break;

        case KeyPress:
        case KeyRelease:
            target_window->on_keyboard(convert_x11_key_event(ev, this->dpy));
            break;

        case ConfigureNotify:
        {
            auto event = convert_x11_configure_event(ev);
            if (event.size != target_window->content.size)
            {
                spdlog::info("Resing window {} to {}x{}", target_window->title, event.size.width, event.size.height);
                target_window->content.resize(event.size.width, event.size.height);

                // TODO - How about I just modify the x11_image with the
                //        needed values? I should modify only the width/height and
                //        byteperline or something.
                XFree(target_window->x11_image);
                target_window->x11_image = XCreateImage(dpy, DefaultVisual(dpy, 0), 24, ZPixmap, 0,
                                                        (char *)target_window->content.buf,
                                                        target_window->content.size.width,
                                                        target_window->content.size.height, 32, 0);

                target_window->on_resize(event);
            }
            break;
        }
        }

        if (target_window->needs_redraw)
        {
            // or call this -
            // XClearArea(display, window, 0, 0, 500, 500, False);
            // and then handle the draw in expose?
            target_window->draw();
            XPutImage(dpy,
                      target_window->x11_window,
                      target_window->gc,
                      target_window->x11_image,
                      0, 0, 0, 0,
                      target_window->x11_image->width,
                      target_window->x11_image->height);
            XFlush(dpy);
        }

        if (this->close_on_last_window && this->windows.size() == 0)
        {
            this->exit_loop = true;
            spdlog::info("No more windows - closing event loop");
        }
    }
}
