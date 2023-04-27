#pragma once

#include <widget.h>

struct Button : Widget {

    enum class States {
        Normal = 1, 
        Hovered = 22,
        ClickedUnreleased = 30
    };

    States state;
    std::string text;

    Button(Position pp, Size size, std::string text);

    virtual auto draw() -> void;
    virtual auto on_hover(const EventMouse &event) -> void;
    virtual auto on_mouse_enter() -> void;
    virtual auto on_mouse_leave() -> void;
    virtual auto on_mouse_click(const EventMouse &event) -> void;
};
