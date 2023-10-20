/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <button.h>
#include <textfield.h>
#include <widget.h>

struct IntegerValidator : public TextField::input_validator {
    virtual auto is_keyboard_input_valid(KeyCodes keycode, int position) -> bool override;
    virtual auto is_string_valid(std::string_view str) -> bool override;
};

struct Spinbox : public TextField {
    std::function<void(Spinbox *, double value)> did_change = {};

    double min_value = 0.0;
    double max_value = 0.0;
    double interval = 1.0;
    bool fractional = false;
    double value = 0;

    Spinbox(Position position, Size size);
    virtual auto draw() -> void override;

    auto increase_value() -> void;
    auto decrease_value() -> void;
    auto set_value(double value) -> void;
    auto set_values(double value, double min, double max) -> void;

  private:
    std::shared_ptr<Button> up_button = {};
    std::shared_ptr<Button> down_button = {};
};
