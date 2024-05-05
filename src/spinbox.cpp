/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "spinbox.h"
#include "bitmap.h"
#include "button.h"
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
    button_position.y = s;
    this->up_button = std::make_shared<Button>(button_position, button_size, "+");
    this->up_button->on_button_click = [this](auto &) { this->increase_value(); };
    this->up_button->set_auto_repeat(500);
    this->down_button = std::make_shared<Button>(button_position, button_size, "-");
    this->down_button->on_button_click = [this](auto &) { this->decrease_value(); };
    this->down_button->set_auto_repeat(500);
    this->validator = std::make_shared<IntegerValidator>();
    add(up_button);
    add(down_button);
}

auto Spinbox::on_resize() -> void {
    auto s = content.size.height / 2;
    auto button_position = Position{content.size.width - s, 0};
    up_button->position = button_position;
    up_button->content.resize(s, s);
    up_button->invalidate();

    button_position.y = s;
    down_button->position = button_position;
    down_button->content.resize(s, s);
    down_button->invalidate();
}

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

auto Spinbox::set_values(double value, double min, double max) -> void {
    this->min_value = min;
    this->max_value = max;
    this->value = value;
}
