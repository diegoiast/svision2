/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <sizepoint.h>

enum class EventPropagation {
    handled = true,
    propagate = false,
};

enum class MouseEvents {
    Unknown,
    Press,
    Release,
    MouseMove,
};

// When possible - keys match the corresponding unicode.
enum class KeyCodes {
    Unknown = 0x00,
    Home = 0x02,
    End = 0x05,

    Backspace = 0x0008,
    Tab = 0x09,
    Return = 0x0a,
    Enter = 0x0d,
    ArrowDown = 0x011,
    ArrowUp = 0x012,
    ArrowRight = 0x13,
    ArrowLeft = 0x18,
    Escape = 0x001b,
    Space = ' ',

    Delete = 0x007F,
    Insert = 0x007E,
    PageDown = 0xff55,
    PageUp = 0xff56,
    ShiftLeft = 0xffe1,
    ShiftRight = 0xffe2,
    ControlLeft = 0xffe3,
    ControlRight = 0xffe4,
};

struct Event {};

struct EventKeyboard : Event {
    int modifiers;
    KeyCodes key;
    bool keydown;

    auto is_control_pressed() const -> bool { return modifiers & 0x1; }
    auto is_shift_pressed() const -> bool { return modifiers & 0x2; }
    auto is_mod_pressed() const -> bool { return modifiers & 0x4; }
};

struct EventMouse : Event {
    MouseEvents type = MouseEvents::Unknown;
    bool pressed = false;
    bool is_local = false;
    int button = -1;
    int x = -1;
    int y = -1;

    auto invalidate() -> auto {
        x = -1;
        y = -1;
    }

    auto is_valid() const -> bool { return x <= 0 || y <= 0; }
};

struct EventResize : Event {
    Size size{-1, -1};
    Position position = {-1, -1};
};
