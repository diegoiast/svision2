/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <widget.h>

class Button;


struct ScrollBar : Widget
{
    ScrollBar(Position position, int length, bool horizontal);

    bool is_horizontal = false;
private:
    std::shared_ptr<Button> up_button = {};
    std::shared_ptr<Button> down_button = {};
};
