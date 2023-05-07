/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <bitmap.h>
#include <events.h>
//#include <platform.h>

#include <list>
#include <memory>
#include <string>

struct Theme;
struct PlatformWindow;
struct Platform;

struct Widget {
    Bitmap content;
    Position position;
    std::shared_ptr<Theme> theme;

    // TODO this should be a shared pointer
    PlatformWindow *window = nullptr;

    bool mouse_over = false;
    bool needs_redraw = true;
    bool has_focus = false;
    bool can_focus = false;
    int focus_index = -1;

    // used for debug. will get removed soon
    bool state_pressed = false;
    Position pos;
    bool unclick_inside;

    Widget(Position pp, Size size, uint32_t color);
    virtual ~Widget();
    virtual auto draw() -> void;
    virtual auto on_hover(const EventMouse &event) -> void;
    virtual auto on_mouse_enter() -> void;
    virtual auto on_mouse_leave() -> void;
    virtual auto on_mouse_click(const EventMouse &event) -> void;
    virtual auto on_focus_change(bool new_state) -> void{};
    virtual auto on_keyboard(const EventKeyboard &) -> void{};

    virtual auto on_remove() -> void{};
};

struct PlatformWindow {
    std::string title;
    Bitmap content;
    uint32_t background_color = 0;
    std::list<std::shared_ptr<Widget>> widgets;
    std::shared_ptr<Widget> last_overed_widget;
    std::shared_ptr<Widget> focus_widget;
    std::shared_ptr<Theme> theme;
    int max_focus_index = 1;

    bool needs_redraw = false;
    Platform *platform = nullptr;

    virtual ~PlatformWindow();

    auto select_next_widget() -> void;
    auto select_widget(std::shared_ptr<Widget> widget) -> void;

    virtual auto draw() -> void;
    virtual auto on_keyboard(const EventKeyboard &) -> void;
    virtual auto on_mouse(const EventMouse &) -> void;
    virtual auto on_resize(const EventResize &) -> void {}
    virtual auto can_close() -> bool { return true; }
    virtual auto invalidate() -> void;
    virtual auto on_close() -> void;

    auto add(std::shared_ptr<Widget> widget) -> std::shared_ptr<Widget>;
};
