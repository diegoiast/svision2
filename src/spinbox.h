/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <widget.h>
#include <textfield.h>
#include <button.h>

struct Spinbox : public TextField {
    double min_value = 0.0;
    double max_value = 0.0;
    bool fractional = false;

    Spinbox(Position position, Size size);
    virtual auto draw() -> void;
  private:
    std::shared_ptr<Button> up_button = {};
    std::shared_ptr<Button> down_button = {};

};
