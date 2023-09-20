/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "spinbox.h"
#include "spdlog/spdlog.h"
#include "fmt/core.h"

Spinbox::Spinbox(Position position, Size size)
    : TextField(position, size)
{
    auto s = content.size.height / 2;
    auto button_size = Size{s, s};
    auto button_position = Position{content.size.width-s, 0};
    padding_end = s + 5;
    this->up_button = std::make_shared<Button>(button_position, button_size, "+");
    button_position.y = s;
    this->down_button = std::make_shared<Button>(button_position, button_size, "-");
    up_button->on_button_click = [this](){
        this->increase_value();
    };
    up_button->set_auto_repeat(500);
    down_button->on_button_click = [this](){
        this->decrease_value();
    };
    down_button->set_auto_repeat(500);

    add(up_button);
    add(down_button);
}

void Spinbox::draw()
{
    TextField::draw();
}


auto Spinbox::increase_value() -> void {
    value += interval;
    set_text(fmt::format("{}", std::round(value)));
}

auto Spinbox::decrease_value() -> void {
    value += interval;
    set_text(fmt::format("{}", std::round(value)));
}

