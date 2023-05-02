/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <bitmap.h>
#include <events.h>
#include <theme.h>

#include <list>
#include <memory>
#include <string>

struct Widget {
    Bitmap content;
    Position position;
    std::shared_ptr<Theme> theme;
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
    virtual auto draw() -> void;
    virtual auto on_hover(const EventMouse &event) -> void;
    virtual auto on_mouse_enter() -> void;
    virtual auto on_mouse_leave() -> void;
    virtual auto on_mouse_click(const EventMouse &event) -> void;
    virtual auto on_focus_change(bool new_state) -> void{};
    virtual auto on_keyboard(const EventKeyboard &) -> void{};
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

    virtual ~PlatformWindow() {}

    virtual auto draw() -> void;

    virtual auto on_keyboard(const EventKeyboard &) -> void;
    virtual auto on_mouse(const EventMouse &) -> void;

    virtual auto on_resize(const EventResize &) -> void {
        // by default, do nothing
    }

    virtual auto can_close() -> bool { return true; }

    auto add(std::shared_ptr<Widget> w) -> std::shared_ptr<Widget> {
        widgets.push_back(w);
        w->theme = theme;
        if (w->focus_index < 0) {
            w->focus_index = max_focus_index;
            max_focus_index++;
        }
        return w;
    }
};
