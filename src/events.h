/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <sizepoint.h>

enum class MouseEvents {
    Unknown,
    Press,
    Release,
    MouseMove,
};

enum class KeyCodes {
    Unknown = 0x00,
    Escape = 0x01,
    Tab = 0x09,
    Return = 0x0a,
    Enter = 0x0d,
    ArrowLeft = 0x18,
    ArrowRight = 0x13,
    ArrowUp = 0x00,
    ArrowDown = 0x00,
    Home = 0x02,
    End = 0x05,
    Delete = 0x007F,
    Backspace = 0x0008,
    Insert = 0x007E,
    Space = ' ',
};

struct Event {};

struct EventKeyboard : Event {
    int modifiers;
    KeyCodes key;
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
