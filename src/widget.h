/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <bitmap.h>
#include <checkboxshape.h>
#include <events.h>
#include <layout.h>

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

    auto on_mouse(const EventMouse &event) -> EventPropagation;
    auto on_mouse_release(const EventMouse &event, std::shared_ptr<Widget> w) -> EventPropagation;
    auto on_mouse_press(const EventMouse &event, std::shared_ptr<Widget> w) -> EventPropagation;

    auto focus_next_widget() -> void;
    auto focus_previous_widget() -> void;
    auto focus_widget(std::shared_ptr<Widget> widget) -> void;
};

struct Widget : std::enable_shared_from_this<Widget>, LayouttItem {
    Bitmap content;
    Position position;
    WidgetCollection widgets;
    std::shared_ptr<Theme> theme;
    Frame frame{FrameStyles::NoFrame, FrameSize::SingleFrame};
    std::shared_ptr<LayouttItem> layout;

    // TODO this should be a weak pointer
    PlatformWindow *window = nullptr;
    Widget *parent = nullptr;

    bool draw_background = true;
    bool read_external_mouse_events = false;
    bool mouse_over = false;
    bool has_focus = false;
    bool can_focus = false;
    int focus_index = -1;

    Widget();
    Widget(Position pp, Size size, uint32_t color);
    virtual ~Widget();

    auto invalidate() -> void;
    virtual auto draw() -> void;
    virtual auto on_mouse(const EventMouse &event) -> EventPropagation;
    virtual auto on_hover(const EventMouse &event) -> void;
    virtual auto on_mouse_enter() -> void;
    virtual auto on_mouse_leave() -> void;
    virtual auto on_mouse_click(const EventMouse &event) -> EventPropagation;
    virtual auto on_focus_change(bool new_state) -> void;
    virtual auto on_keyboard(const EventKeyboard &) -> EventPropagation;
    virtual auto on_remove() -> void;

    // TODO - make sure this T derieves from `Widget`
    template <typename T> auto add(T widget) -> T {
        widgets.add(widget, window);
        widget->parent = this;
        return widget;
    }

    // TODO - make sure this T derieves from `Widget`
    template <typename T, typename... Args> auto add_new(Args &&...args) -> std::shared_ptr<T> {
        return add(std::make_shared<T>(std::forward<Args>(args)...));
    };

    auto get_theme() -> std::shared_ptr<Theme>;
    auto show() -> void;
    auto hide() -> void;
    auto is_visible() const -> bool { return is_widget_visible; }

    auto relayout(Position position, const Size size) -> void override {
        this->position = position;
        content.resize(size);
        needs_redraw = true;

        if (layout) {
            layout->relayout(position, size);
        }
    }

    friend class PlatformWindow;
    friend class WidgetCollection;

  protected:
    bool needs_redraw = true;
    bool is_widget_visible = true;
};

struct PlatformWindow {
    std::string title;
    Widget main_widget;

    bool is_visible = false;
    bool needs_redraw = false;
    Platform *platform = nullptr;

    virtual ~PlatformWindow();

    auto focus_next_widget() -> void {
        auto l = main_widget.widgets.focused_widget;
        main_widget.widgets.focus_next_widget();
        if (l != main_widget.widgets.focused_widget)
            invalidate();
    }

    auto focus_previous_widget() -> void {
        auto l = main_widget.widgets.focused_widget;
        main_widget.widgets.focus_previous_widget();
        if (l != main_widget.widgets.focused_widget)
            invalidate();
    }

    auto focus_widget(std::shared_ptr<Widget> widget) -> void {
        auto l = main_widget.widgets.focused_widget;
        main_widget.widgets.focus_widget(widget);
        if (l != main_widget.widgets.focused_widget)
            invalidate();
    }

    virtual auto draw() -> void;
    virtual auto on_keyboard(const EventKeyboard &) -> void;
    virtual auto on_mouse(const EventMouse &) -> void;
    virtual auto on_resize(const EventResize &) -> void;
    virtual auto can_close() -> bool { return true; }
    virtual auto invalidate() -> void;
    virtual auto on_close() -> void;

    // TODO - make sure this T derieves from `Widget`
    template <typename T> auto add(T widget) -> T {
        main_widget.widgets.add(widget, this);
        return widget;
    };

    // TODO - make sure this T derieves from `Widget`
    template <typename T, typename... Args> auto add_new(Args &&...args) -> std::shared_ptr<T> {
        return add(std::make_shared<T>(std::forward<Args>(args)...));
    };

    // TODO - make sure this T derieves from `Widget`
    template <typename T, typename... Args>
    auto add_new_to_layout(std::shared_ptr<LayouttItem> layout, Args &&...args)
        -> std::shared_ptr<T> {
        auto widget = add(std::make_shared<T>(std::forward<Args>(args)...));
        if (layout) {
            layout->add(widget);
        }
        return widget;
    };
};
