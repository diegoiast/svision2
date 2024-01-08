/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "widget.h"
#include <functional>

struct Checkbox;

struct RadioButtonGroup : Widget {
  public:
    RadioButtonGroup(Position pos, int width, const std::vector<std::string> &items);

    std::function<void(int index, Checkbox &button)> on_selected = {};
    std::vector<std::shared_ptr<Checkbox>> radio_buttons = {};
};
