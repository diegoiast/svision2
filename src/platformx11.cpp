/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <spdlog/spdlog.h>

#include "events.h"
#include "platformx11.h"
#include "theme.h"
#include "widget.h"

#include "platformx11-keycodes.h"

auto convert_x11_key_event(XEvent &ev, Display *dpy) -> EventKeyboard {
    auto event = EventKeyboard();
    char buf[20];
    KeySym keySym;
    (void)XLookupString((XKeyEvent *)&ev, buf, 20, &keySym, nullptr);

    // TODO binary search could be nice.
    for (auto i = 0; X11_KEYCODES[i] != 0; i += 2) {
        /* Map XKB KeySym to our custom key code value */
        if (X11_KEYCODES[i] == keySym) {
            event.key = (KeyCodes)X11_KEYCODES[i + 1];
            break;
        }
    }

    if (event.key == KeyCodes::Unknown) {
        spdlog::critical("Keycode 0x{0:x} is not detected, will send 0x{1:x}", keySym,
                         (int)event.key);
    }
    auto m = ev.xkey.state;
    event.keydown = ev.type == KeyPress;
    event.modifiers = (!!(m & ControlMask)) | (!!(m & ShiftMask) << 1) | (!!(m & Mod1Mask) << 2) |
                      (!!(m & Mod4Mask) << 3);
    return event;
}

auto convert_x11_mouse_event(const XEvent &ev, Display *dpy) -> EventMouse {
    auto event = EventMouse();
    switch (ev.type) {
    case MotionNotify:
        event.type = MouseEvents::MouseMove;
        event.x = ev.xmotion.x;
        event.y = ev.xmotion.y;
        break;
    case ButtonPress:
        event.type = MouseEvents::Press;
        event.button = ev.xbutton.button;
        event.pressed = true;
        break;
    case ButtonRelease:
        event.type = MouseEvents::Release;
        event.button = ev.xbutton.button;
        event.pressed = false;
        break;
    }
    return event;
}

auto convert_x11_configure_event(const XEvent &ev) -> EventResize {
    auto event = EventResize();
    event.size.width = ev.xconfigure.width;
    event.size.height = ev.xconfigure.height;
    event.position.x = ev.xconfigure.x;
    event.position.y = ev.xconfigure.y;
    return event;
}

struct PlatformWindowX11 : public PlatformWindow {
    Window x11_window;
    XImage *x11_image = nullptr;

    // TODO - is a GContent a global variable, and not per window?
    GC gc;

    virtual ~PlatformWindowX11() override {
        XFree(x11_image);
        // XDestroyWindow(dpy, x11_window);
        // XFreeGC(dpy, gc);
    }
};

auto PlatformX11::init() -> void {
    spdlog::set_level(spdlog::level::info);
    dpy = XOpenDisplay(NULL);
    screen = DefaultScreen(dpy);

    wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    if (wmDeleteMessage == None) {
        spdlog::error("Could not get redraw atom from WM");
        return;
    }

#if 0
    default_theme = std::make_shared<ThemePlasma>();
#else
           default_theme = std::make_shared<ThemeRedmond>();
    // default_theme = std::make_shared<ThemeVision>();
#endif
    spdlog::info("PlatformX11 initialized");
}

auto PlatformX11::done() -> void {
    for (auto w : windows) {
        XDestroyWindow(dpy, w.second->x11_window);
        XFreeGC(dpy, w.second->gc);
    }
    XCloseDisplay(dpy);
}

auto PlatformX11::open_window(int x, int y, int width, int height, const std::string title)
    -> std::shared_ptr<PlatformWindow> {
    auto window = std::make_shared<PlatformWindowX11>();
    window->title = title;
    window->x11_window = XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 0, 0, width, height, 0,
                                             BlackPixel(dpy, screen), WhitePixel(dpy, screen));
    window->gc = XCreateGC(dpy, window->x11_window, 0, 0);
    XSelectInput(dpy, window->x11_window,
                 ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XStoreName(dpy, window->x11_window, title.c_str());
    XSetWindowBackgroundPixmap(dpy, window->x11_window, None);
    wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, window->x11_window, &wmDeleteMessage, 1);

    window->content.resize(width, height);
    window->theme = default_theme;
    window->platform = this;
    window->x11_image =
        XCreateImage(dpy, DefaultVisual(dpy, 0), 24, ZPixmap, 0, (char *)window->content.buf,
                     window->content.size.width, window->content.size.height, 32, 0);

    windows[window->x11_window] = window;
    return window;
}

auto PlatformX11::show_window(std::shared_ptr<PlatformWindow> w) -> void {
    auto window = std::dynamic_pointer_cast<PlatformWindowX11>(w);
    XMapWindow(dpy, window->x11_window);
    XSync(dpy, window->x11_window);
}

auto PlatformX11::invalidate(PlatformWindow &window) -> void {
    auto windowX11 = static_cast<PlatformWindowX11 *>(&window);

    XEvent exppp;
    exppp.type = Expose;
    exppp.xexpose.window = windowX11->x11_window;
    XSendEvent(dpy, windowX11->x11_window, False, ExposureMask, &exppp);
    XFlush(dpy);
}

auto PlatformX11::main_loop() -> void {
    XEvent ev;
    int pending;
    Position last_mouse_position;

    while ((pending = XPending(dpy) || !this->exit_loop)) {
        auto k = XNextEvent(dpy, &ev);
        std::shared_ptr<PlatformWindowX11> target_window;

        if (windows.find(ev.xany.window) == windows.end()) {
            spdlog::critical("Window =({}) no longer alive, ignoring event {}", ev.xany.window,
                             ev.type);
            continue;
        }
        target_window = windows.at(ev.xany.window);
        spdlog::debug("Sending message {} to \"{}\"", ev.type, target_window->title);

        switch (ev.type) {
        case Expose:
            target_window->needs_redraw = true;
            break;

        case ClientMessage:
            spdlog::debug("Atom {}", ev.xclient.data.l[0]);
            if (ev.xclient.data.l[0] == wmDeleteMessage) {
                if (target_window->can_close()) {
                    XDestroyWindow(dpy, target_window->x11_window);
                }
            }
            break;

        case UnmapNotify:
            break;
        case DestroyNotify:
            target_window->on_close();
            windows.erase(ev.xany.window);
            break;
        case ButtonPress:
        case ButtonRelease: {
            // TODO - verify this!
            // mouse events on X11 do not tell you the position of the click, we
            // need to save that data and pass it along
            auto event = convert_x11_mouse_event(ev, this->dpy);
            event.x = last_mouse_position.x;
            event.y = last_mouse_position.y;
            target_window->on_mouse(event);
        } break;

        case MotionNotify: {
            auto event = convert_x11_mouse_event(ev, this->dpy);
            target_window->on_mouse(event);
            last_mouse_position.x = event.x;
            last_mouse_position.y = event.y;
        } break;

        case KeyPress:
        case KeyRelease:
            target_window->on_keyboard(convert_x11_key_event(ev, this->dpy));
            break;

        case ConfigureNotify: {
            auto event = convert_x11_configure_event(ev);
            if (event.size != target_window->content.size) {
                spdlog::info("Resing window {} to {}x{}", target_window->title, event.size.width,
                             event.size.height);
                target_window->content.resize(event.size.width, event.size.height);

                // TODO - How about I just modify the x11_image with the
                //        needed values? I should modify only the width/height and
                //        byteperline or something.
                XFree(target_window->x11_image);
                target_window->x11_image = XCreateImage(
                    dpy, DefaultVisual(dpy, 0), 24, ZPixmap, 0, (char *)target_window->content.buf,
                    target_window->content.size.width, target_window->content.size.height, 32, 0);

                target_window->on_resize(event);
            }
            break;
        }
        }

        if (target_window->needs_redraw) {
            // or call this -
            // XClearArea(display, window, 0, 0, 500, 500, False);
            // and then handle the draw in expose?
            target_window->draw();
            XPutImage(dpy, target_window->x11_window, target_window->gc, target_window->x11_image,
                      0, 0, 0, 0, target_window->x11_image->width,
                      target_window->x11_image->height);
            XFlush(dpy);
        }

        if (this->close_on_last_window && this->windows.size() == 0) {
            this->exit_loop = true;
            spdlog::info("No more windows - closing event loop");
        }
    }
}
