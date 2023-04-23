// https://zserge.com/posts/fenster/

#include <memory>
#include <vector>
#include <map>
#include <iostream>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <time.h>

struct PlatformWindow
{
    uint32_t *buf;
    std::string title;
};

struct PlatformWindowX11 : public PlatformWindow
{
    Window w;
    GC gc;
    XImage *img;
};

class PlatformX11
{
    std::map<Window, std::shared_ptr<PlatformWindow>> windows;
    Display *dpy = nullptr;
    int screen = -1;
    bool exit_loop = false;

public:
    auto init() -> void;
    auto done() -> void;
    auto open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow>;
    void main_loop();
};

static int FENSTER_KEYCODES[124] = {XK_BackSpace, 8, XK_Delete, 127, XK_Down, 18, XK_End, 5, XK_Escape, 27, XK_Home, 2, XK_Insert, 26, XK_Left, 20, XK_Page_Down, 4, XK_Page_Up, 3, XK_Return, 10, XK_Right, 19, XK_Tab, 9, XK_Up, 17, XK_apostrophe, 39, XK_backslash, 92, XK_bracketleft, 91, XK_bracketright, 93, XK_comma, 44, XK_equal, 61, XK_grave, 96, XK_minus, 45, XK_period, 46, XK_semicolon, 59, XK_slash, 47, XK_space, 32, XK_a, 65, XK_b, 66, XK_c, 67, XK_d, 68, XK_e, 69, XK_f, 70, XK_g, 71, XK_h, 72, XK_i, 73, XK_j, 74, XK_k, 75, XK_l, 76, XK_m, 77, XK_n, 78, XK_o, 79, XK_p, 80, XK_q, 81, XK_r, 82, XK_s, 83, XK_t, 84, XK_u, 85, XK_v, 86, XK_w, 87, XK_x, 88, XK_y, 89, XK_z, 90, XK_0, 48, XK_1, 49, XK_2, 50, XK_3, 51, XK_4, 52, XK_5, 53, XK_6, 54, XK_7, 55, XK_8, 56, XK_9, 57};

auto PlatformX11::init() -> void
{
    dpy = XOpenDisplay(NULL);
    screen = DefaultScreen(dpy);
}

auto PlatformX11::done() -> void
{
    // TODO close all windows
    // TODO close connection to X server
}

auto PlatformX11::open_window(int x, int y, int width, int height, const  std::string title) -> std::shared_ptr<PlatformWindow>
{
    auto window = std::make_shared<PlatformWindowX11>();
    window->title = title;
    window->w = XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 0, 0, width,
                                    height, 0, BlackPixel(dpy, screen),
                                    WhitePixel(dpy, screen));
    window->gc = XCreateGC(dpy, window->w, 0, 0);
    XSelectInput(dpy, window->w,
                 ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
                     ButtonReleaseMask | PointerMotionMask);
    XStoreName(dpy, window->w, title.c_str());
    XMapWindow(dpy, window->w);
    XSync(dpy, window->w);
    window->img = XCreateImage(dpy, DefaultVisual(dpy, 0), 24, ZPixmap, 0,
                               (char *)window->buf, width, height, 32, 0);

    windows[window->w] = window;
    return window;
}

auto PlatformX11::main_loop() -> void
{
    XEvent ev;
    int pending;

    // XPutImage(f->dpy, f->w, f->gc, f->img, 0, 0, 0, 0, f->width, f->height);
    // XFlush(f->dpy);
    while (pending = XPending(dpy) || !this->exit_loop)
    {
        auto k = XNextEvent(dpy, &ev);
        auto target_window = windows.at(ev.xany.window);
        std::cout  << "Sending message " << ev.type << " to " << target_window->title << std::endl;

        switch (ev.type)
        {
        case DestroyNotify:
            windows.extract(ev.xany.window);
            break;
        case ButtonPress:
        case ButtonRelease:
            // f->mouse = (ev.type == ButtonPress);
            break;
        case MotionNotify:
            // f->x = ev.xmotion.x, f->y = ev.xmotion.y;
            break;
        case KeyPress:
        case KeyRelease:
            // int m = ev.xkey.state;
            // int k = XkbKeycodeToKeysym(f->dpy, ev.xkey.keycode, 0, 0);
            // for (unsigned int i = 0; i < 124; i += 2)
            // {
            //     if (FENSTER_KEYCODES[i] == k)
            //     {
            //         f->keys[FENSTER_KEYCODES[i + 1]] = (ev.type == KeyPress);
            //         break;
            //     }
            // }
            // f->mod = (!!(m & ControlMask)) | (!!(m & ShiftMask) << 1) |
            //          (!!(m & Mod1Mask) << 2) | (!!(m & Mod4Mask) << 3);
            break;
        }
    }
}

int main()
{
    auto platform = PlatformX11();
    platform.init();
    auto w1 = platform.open_window(100, 100, 640, 480, "test 1");
    auto w2 = platform.open_window(300, 300, 640, 480, "test 2");

    platform.main_loop();
    return 0;
}