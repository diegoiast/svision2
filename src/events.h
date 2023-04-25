#pragma once

struct Event
{
};

struct EventKeyboard : public Event
{
    int modifiers;
    int key;
    bool keydown;
};

struct EventMouse : public Event
{
    bool pressed = false;
    int button = -1;
    int x = -1;
    int y = -1;
};

struct EventResize : public Event
{
    Size size{-1, -1};
    Position position = {-1, -1};
};
