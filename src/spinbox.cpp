/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "spinbox.h"
#include "fmt/core.h"

#include <ciso646>
#include <iostream>
#include <sstream>

auto IntegerValidator::is_keyboard_input_valid(KeyCodes keycode, int position) -> bool {
    auto literal = (int)keycode;
    if (literal == '-' or literal == '+') {
        return (position == 0);
    }

    if (literal >= '0' and literal <= '9') {
        return true;
    }

    return false;
}

auto IntegerValidator::is_string_valid(std::string_view str) -> bool {
    std::istringstream iss(std::string{str});
    double value;
    iss >> value;
    return !iss.fail() && iss.eof();
}

Spinbox::Spinbox(Position position, Size size) : TextField(position, size) {
    auto s = content.size.height / 2;
    auto button_size = Size{s, s};
    auto button_position = Position{content.size.width - s, 0};
    padding_end = s + 5;
    this->up_button = std::make_shared<Button>(button_position, button_size, "+");
    button_position.y = s;
    this->down_button = std::make_shared<Button>(button_position, button_size, "-");
    up_button->on_button_click = [this]() { this->increase_value(); };
    up_button->set_auto_repeat(500);
    down_button->on_button_click = [this]() { this->decrease_value(); };
    down_button->set_auto_repeat(500);

    this->validator = std::make_shared<IntegerValidator>();

    add(up_button);
    add(down_button);
}

void Spinbox::draw() { TextField::draw(); }

auto Spinbox::increase_value() -> void {
    std::istringstream iss(get_text());
    iss >> value;
    if (iss.fail() and iss.eof()) {
        value = 0;
    }
    value += interval;
    set_text(fmt::format("{}", std::round(value)));
    if (did_change) {
        did_change(this, value);
    }
}

auto Spinbox::decrease_value() -> void {
    std::istringstream iss(get_text());
    iss >> value;
    if (iss.fail() and iss.eof()) {
        value = 0;
    }
    value -= interval;
    set_text(fmt::format("{}", std::round(value)));
    if (did_change) {
        did_change(this, value);
    }
}

auto Spinbox::set_value(double value) -> void {
    if (fabs(value - this->value) < 0.00001) {
        return;
    }
    if (value > max_value) {
        value = max_value;
    }
    if (value < min_value) {
        value = min_value;
    }
    this->value = value;
    set_text(fmt::format("{}", std::round(value)));
    if (did_change) {
        did_change(this, value);
    }
}

auto Spinbox::set_values(double min, double max, double value) -> void {
    this->min_value = min;
    this->max_value = max;
    this->value = value;
}
