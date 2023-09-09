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
struct Platform;
struct PlatformWindow;
struct Widget;

struct WidgetCollection {
    std::list<std::shared_ptr<Widget>> widgets;
    std::shared_ptr<Widget> last_overed_widget;
    std::shared_ptr<Widget> focused_widget;
    int max_focus_index = 1;

    auto add(std::shared_ptr<Widget> widget, PlatformWindow *window) -> std::shared_ptr<Widget>;

    auto on_mouse(const EventMouse &event) -> void;
    auto on_mouse_release(const EventMouse &event, std::shared_ptr<Widget> w) -> void;
    auto on_mouse_press(const EventMouse &event, std::shared_ptr<Widget> w) -> void;

    auto focus_next_widget() -> void;
    auto focus_previous_widget() -> void;
    auto focus_widget(std::shared_ptr<Widget> widget) -> void;
};

struct Widget {
    Bitmap content;
    Position position;
    WidgetCollection widgets;
    std::shared_ptr<Theme> theme;

    // TODO this should be a shared pointer
    PlatformWindow *window = nullptr;
    Widget *parent = nullptr;

    bool read_external_mouse_events = false;
    bool mouse_over = false;
    bool has_focus = false;
    bool can_focus = false;
    int focus_index = -1;

    Widget(Position pp, Size size, uint32_t color);
    virtual ~Widget();

    auto invalidate() -> void;
    virtual auto draw() -> void;
    virtual auto on_mouse(const EventMouse &event) -> void;
    virtual auto on_hover(const EventMouse &event) -> void;
    virtual auto on_mouse_enter() -> void;
    virtual auto on_mouse_leave() -> void;
    virtual auto on_mouse_click(const EventMouse &event) -> void;
    virtual auto on_focus_change(bool new_state) -> void{};
    virtual auto on_keyboard(const EventKeyboard &) -> void{};
    virtual auto on_remove() -> void{};

    // TODO - make sure this T derieves from `Widget`
    template <typename T> auto add(T widget) -> T {
        widgets.add(widget, window);
        widget->parent = this;
        return widget;
    }

    auto get_theme() -> std::shared_ptr<Theme>;

    friend class PlatformWindow;
    friend class WidgetCollection;

  private:
    bool needs_redraw = true;
};

struct PlatformWindow {
    std::string title;
    Bitmap content;
    WidgetCollection widgets;
    std::shared_ptr<Theme> theme;

    bool needs_redraw = false;
    Platform *platform = nullptr;

    virtual ~PlatformWindow();

    auto focus_next_widget() -> void { widgets.focus_next_widget(); }
    auto focus_previous_widget() -> void { widgets.focus_previous_widget(); }
    auto focus_widget(std::shared_ptr<Widget> widget) -> void { widgets.focus_widget(widget); }

    virtual auto draw() -> void;
    virtual auto on_keyboard(const EventKeyboard &) -> void;
    virtual auto on_mouse(const EventMouse &) -> void;
    virtual auto on_resize(const EventResize &) -> void {}
    virtual auto can_close() -> bool { return true; }
    virtual auto invalidate() -> void;
    virtual auto on_close() -> void;

    // TODO - make sure this T derieves from `Widget`
    template <typename T> auto add(T widget) -> T {
        widgets.add(widget, this);
        widget->window = this;
        widget->theme = theme;
        return widget;
    }
};
