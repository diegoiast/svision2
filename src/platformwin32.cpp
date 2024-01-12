/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max

#include "spdlog/spdlog.h"

#include "events.h"
#include "fontproviderfreetype.h"
#include "platformwin32.h"
#include "theme.h"
#include "widget.h"

extern "C" int main(int, char **);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    int argc = __argc;
    char **argv = __argv;
    int result = main(argc, argv);
    return result;
}

static std::wstring StringToWideString(const std::string &narrowStr) {
    int wideStrLength = MultiByteToWideChar(CP_UTF8, 0, narrowStr.c_str(), -1, nullptr, 0);
    if (wideStrLength == 0) {
        return L"";
    }

    std::wstring wideStr(wideStrLength, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, narrowStr.c_str(), -1, &wideStr[0], wideStrLength);

    return wideStr;
}

static auto convert_win32_mouse_event(UINT msg, WPARAM wParam, LPARAM lParam) -> EventMouse {
    auto event = EventMouse();
    switch (msg) {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
        event.type = MouseEvents::Press;
        event.pressed = true;
        if (wParam & MK_LBUTTON) {
            event.button = 1;
        }
        if (wParam & MK_RBUTTON) {
            event.button = 2;
        }
        if (wParam & MK_MBUTTON) {
            event.button = 3;
        }
        event.y = HIWORD(lParam);
        event.x = LOWORD(lParam);
        break;

    case WM_LBUTTONUP:
        event.type = MouseEvents::Release;
        event.button = 1;
        event.y = HIWORD(lParam);
        event.x = LOWORD(lParam);
        break;
    case WM_RBUTTONUP:
        event.type = MouseEvents::Release;
        event.button = 2;
        event.y = HIWORD(lParam);
        event.x = LOWORD(lParam);
        break;
    case WM_MBUTTONUP:
        event.type = MouseEvents::Release;
        event.button = 3;
        event.y = HIWORD(lParam);
        event.x = LOWORD(lParam);
        break;
    case WM_XBUTTONUP:
        event.type = MouseEvents::Release;
        event.button = 4;
        event.y = HIWORD(lParam);
        event.x = LOWORD(lParam);
        break;

    case WM_LBUTTONDBLCLK:
    case WM_XBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
        break;

    case WM_MOUSEWHEEL:
        break;

    case WM_MOUSEMOVE:
        event.type = MouseEvents::MouseMove;
        event.y = HIWORD(lParam);
        event.x = LOWORD(lParam);
        break;
    }

    //    spdlog::info("Mouse event at {}x{}", event.x, event.y);
    return event;
}

#include <platformwin32-keycodes.h>

static auto convert_win32_keyboard_event(UINT msg, WPARAM wParam, LPARAM lParam) -> EventKeyboard {
    auto event = EventKeyboard();
    event.modifiers = ((GetKeyState(VK_CONTROL) & 0x8000) >> 15) |
                      ((GetKeyState(VK_SHIFT) & 0x8000) >> 14) |
                      ((GetKeyState(VK_MENU) & 0x8000) >> 13) |
                      (((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000) >> 12);
    event.keydown = !((lParam >> 31) & 1);

    // TODO binary search could be nice.
    event.key = KeyCodes::Unknown;
    for (auto i = 0; WIN32_KEYCODES[i] != 0; i += 2) {
        /* Map XKB KeySym to our custom key code value */
        if (WIN32_KEYCODES[i] == wParam) {
            event.key = (KeyCodes)WIN32_KEYCODES[i + 1];
            break;
        }
    }
    return event;
}

static auto convert_win32_resize_event(UINT msg, WPARAM wParam, LPARAM lParam) -> EventResize {
    auto event = EventResize();

    switch (msg) {
    case WM_MOVE:
        event.position.x = LOWORD(lParam);
        event.position.y = HIWORD(lParam);
        break;

    case WM_MOVING:
        break;

    case WM_SIZE:
        event.size.width = LOWORD(lParam);
        event.size.height = HIWORD(lParam);
        break;

    case WM_SIZING:
        break;

    default:
        break;
    }
    return event;
}

static auto convert_mouse_cursor_to_win32(MouseCursor cursor) -> LPSTR {
    switch (cursor) {
    case MouseCursor::Inherit:
        return nullptr;
    case MouseCursor::Arrow:
        return IDC_ARROW;
    case MouseCursor::Wait:
        return IDC_APPSTARTING;
    case MouseCursor::Buzy:
        return IDC_WAIT;
    case MouseCursor::Beam:
        return IDC_IBEAM;
    case MouseCursor::SizeVertical:
        return IDC_SIZENS;
    case MouseCursor::SizeHorizonal:
        return IDC_SIZEWE;
    case MouseCursor::SizeDiagonalRight:
        return IDC_SIZENESW;
    case MouseCursor::SizeDiagonalLeft:
        return IDC_SIZENWSE;
    case MouseCursor::SizeAll:
        return IDC_SIZEALL;
    case MouseCursor::SplitHorizontal:
        return IDC_ARROW;
    case MouseCursor::SplitVertical:
        return IDC_ARROW;
    case MouseCursor::NoCursor:
        return IDC_NO;
    case MouseCursor::Pointer:
        return IDC_HAND;
    case MouseCursor::Forbidden:
        return IDC_NO;
    case MouseCursor::WhatsThis:
        return IDC_HELP;
    case MouseCursor::Cross:
        return IDC_CROSS;
    default:
        break;
    }
}

struct PlatformWindowWin32 : public PlatformWindow {
    HWND hwnd;
};

static auto win32_paint_window(PlatformWindowWin32 *window) -> void {
    if (window->main_widget.content.buffer.data() == nullptr ||
        window->main_widget.content.size.height <= 0) {
        spdlog::warn("win32_paint_window: Window has invalid size! ptr={}, size={}x{}",
                     fmt::ptr(window->main_widget.content.buffer.data()),
                     window->main_widget.content.size.width,
                     window->main_widget.content.size.height);
        return;
    }

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(window->hwnd, &ps);
    HDC memdc = CreateCompatibleDC(hdc);
    HBITMAP hbmp = CreateCompatibleBitmap(hdc, window->main_widget.content.size.width,
                                          window->main_widget.content.size.height);
    HGDIOBJ oldbmp = SelectObject(memdc, hbmp);
    BITMAPINFO bi = {{sizeof(bi), window->main_widget.content.size.width,
                      -window->main_widget.content.size.height, 1, 32, BI_RGB}};
    bi.bmiColors[0].rgbRed = 0xff;
    bi.bmiColors[0].rgbGreen = 0xff;
    bi.bmiColors[0].rgbBlue = 0xff;
    bi.bmiColors[0].rgbReserved = 0xff;

    SetDIBitsToDevice(memdc, 0, 0, window->main_widget.content.size.width,
                      window->main_widget.content.size.height, 0, 0, 0,
                      window->main_widget.content.size.height,
                      window->main_widget.content.buffer.data(), (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    BitBlt(hdc, 0, 0, window->main_widget.content.size.width,
           window->main_widget.content.size.height, memdc, 0, 0, SRCCOPY);
    SelectObject(memdc, oldbmp);
    DeleteObject(hbmp);
    DeleteDC(memdc);
    EndPaint(window->hwnd, &ps);
}

static auto total_open_windows = 0;

static LRESULT CALLBACK svision_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    PlatformWindowWin32 *window =
        (struct PlatformWindowWin32 *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (window == nullptr) {
        spdlog::critical("Window =({}), message={}: is not managed by us, ignoring",
                         (long long)hwnd, msg);
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    auto title = window->title;
    spdlog::debug("Sending message {} to \"{}\"", msg, title);
    switch (msg) {
    case WM_SHOWWINDOW:
        total_open_windows += 1;
        break;

    case WM_DESTROY:
        break;

    case WM_PAINT:
        window->draw();
        win32_paint_window(window);
        break;

    case WM_CLOSE:
        if (window->can_close()) {
            DestroyWindow(hwnd);
            total_open_windows -= 1;
        }
        break;

    //      case WM_MOVE:
    //      case WM_MOVING:
    case WM_SIZE:
        //      case WM_SIZING:
        {
            auto event = convert_win32_resize_event(msg, wParam, lParam);
            if (event.size != window->main_widget.content.size) {
                window->needs_redraw = true;
                window->main_widget.content.resize(event.size.width, event.size.height);
                window->on_resize(event);
            }
        }
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_XBUTTONDBLCLK:
    case WM_MOUSEMOVE:
        window->on_mouse(convert_win32_mouse_event(msg, wParam, lParam));
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
        window->on_keyboard(convert_win32_keyboard_event(msg, wParam, lParam));
        break;

    case WM_CHAR: {
        auto event = EventKeyboard();
        event.keydown = true;
        event.key = static_cast<KeyCodes>(wParam);
        window->on_keyboard(event);
    } break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    if (window->needs_redraw) {
        InvalidateRect(window->hwnd, 0, 1);
    }
    return 0;
}
static constexpr auto WINDOW_CLASS_NAME = L"svision2";

PlatformWin32::PlatformWin32() {}

auto PlatformWin32::init() -> void {
    spdlog::set_level(spdlog::level::info);

    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = svision_wndproc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    RegisterClassExW(&wc);

    this->default_font = std::make_shared<FontProviderFreetype>(default_font_file);
    default_theme = std::make_shared<ThemeVision>(*this->default_font);
    spdlog::info("PlatformWin32 initialized");
}

auto PlatformWin32::done() -> void {}

auto PlatformWin32::open_window(int x, int y, int width, int height, const std::string &title)
    -> std::shared_ptr<PlatformWindow> {
    auto hInstance = GetModuleHandle(nullptr);
    auto window_rect = RECT{0, 0, width, height};
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

    auto windowWidth = window_rect.right - window_rect.left;
    auto windowHeight = window_rect.bottom - window_rect.top;

    auto window = std::make_shared<PlatformWindowWin32>();
    window->title = title;
    window->hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, WINDOW_CLASS_NAME,
                                   StringToWideString(window->title).c_str(), WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, NULL,
                                   NULL, hInstance, NULL);
    if (window->hwnd == nullptr) {
        return nullptr;
    }
    SetWindowLongPtr(window->hwnd, GWLP_USERDATA, (LONG_PTR)window.get());
    window->main_widget.theme = default_theme;
    window->platform = this;
    return window;
}

auto PlatformWin32::show_window(std::shared_ptr<PlatformWindow> w) -> void {
    auto window = std::dynamic_pointer_cast<PlatformWindowWin32>(w);
    ShowWindow(window->hwnd, SW_NORMAL);
    UpdateWindow(window->hwnd);
}

auto PlatformWin32::set_cursor(PlatformWindow &window, MouseCursor cursor) -> void {
    auto x11_window = static_cast<PlatformWindowWin32 *>(&window);
    HCURSOR win32_cursor;

    if (cursor_cache.find(cursor) == cursor_cache.end()) {
        auto c = convert_mouse_cursor_to_win32(cursor);
        win32_cursor = LoadCursor(nullptr, c);
        cursor_cache[cursor] = win32_cursor;
        spdlog::info("Caching new win32 cursor - {}, ({} so far)", (int)cursor,
                     cursor_cache.size());
    } else {
        win32_cursor = (HCURSOR)cursor_cache[cursor];
    }

    //  spdlog::info("Setting new cursor - {}", (int)cursor);
    // auto c = convert_mouse_cursor_to_win32(cursor);
    //    HCURSOR cc = LoadCursor(nullptr, c);
    SetCursor(win32_cursor);
}

auto PlatformWin32::clear_cursor_cache() -> void {
    // win32 cursors do not need to be destroyed using `DestroyCursor()`
    cursor_cache.clear();
}

auto PlatformWin32::invalidate(PlatformWindow &w) -> void {
    auto window = static_cast<PlatformWindowWin32 *>(&w);
    InvalidateRect(window->hwnd, 0, 1);
}

auto PlatformWin32::main_loop() -> void {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) && !this->exit_loop) {
        if (msg.message == WM_QUIT) {
            spdlog::debug("Closing event loop, due to request from WM");
            return;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (this->close_on_last_window && total_open_windows == 0) {
            spdlog::info("No more windows - closing event loop");
            PostQuitMessage(0);
            this->exit_loop = true;
        }
    }
}
