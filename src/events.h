#pragma once

enum class MouseEvents {
    Unknown,
    Press,
    Release,
    MouseMove,
};

struct Event {};

struct EventKeyboard : Event {
    int modifiers;
    int key;
    bool keydown;
};

struct EventMouse : Event {
    MouseEvents type = MouseEvents::Unknown;
    bool pressed = false;
    bool is_local = false;
    int button = -1;
    int x = -1;
    int y = -1;

    auto invalidate() -> auto{
        x = -1;
        y = -1;
    }

    auto is_valid() -> const bool { return x < 0 || y < 0; }
};

struct EventResize : Event {
    Size size{-1, -1};
    Position position = {-1, -1};
};
