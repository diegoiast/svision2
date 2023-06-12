/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <widget.h>

class Button;

struct ScrollBar : Widget {
    ScrollBar(Position position, int length, bool horizontal, int max=100);

    bool is_horizontal = false;
    int minimun = 0;
    int maximum = 100;
    int value = maximum;
    int step = (maximum - minimun) / 10;

    auto draw() -> void override;
    auto set_value(int minimum, int maximum, int step=0 ) -> void;
    auto step_up() -> void;
    auto step_down() -> void;

  private:
    std::shared_ptr<Button> up_button = {};
    std::shared_ptr<Button> down_button = {};
};
