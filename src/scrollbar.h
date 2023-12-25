/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <functional>
#include <widget.h>

class Button;

struct ScrollBar : Widget {
    std::function<void(ScrollBar *, int value)> did_change = {};

    ScrollBar(Position position, int length, bool horizontal, int max = 100);

    bool is_horizontal = false;
    int minimum = 0;
    int maximum = 100;
    int value = maximum;
    int step = (maximum - minimum) / 10;

    auto draw() -> void override;
    auto on_resize() -> void override;

    auto set_value(int value) -> void;
    auto set_values(int minimum, int maximum, int value, int step = 0) -> void;
    auto step_up() -> void;
    auto step_down() -> void;

    std::shared_ptr<Button> up_button = {};
    std::shared_ptr<Button> down_button = {};

  private:
    auto update_thumb_size() -> void;
    int thumb_size = 0;
    int thumb_position = 0;

};
