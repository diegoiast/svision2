/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

// I wanted to use "None" - but this is already defined in X11
enum class FrameStyles { NoFrame, Normal, Reversed, Disabled, Hover };
enum class FrameSize { NoFrame, SingleFrame, DoubleFrame, TrippleFrame };

enum class CheckboxShape {
    Checkbox,
    RadioButton,
};

struct ItemStatus {
    bool is_selected = false;
    bool is_active = false;
};

struct Frame {
    FrameStyles style;
    FrameSize size;
};
