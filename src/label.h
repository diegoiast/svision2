/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <theme.h>
#include <widget.h>

struct Label : public Widget {
    std::string text;

    Label(std::string_view text) : Widget({}, {}, 0) {
        this->text = text;
        this->content.resize(1, 1);
    }

    Label(Position pp, Size size, std::string_view text) : Widget(pp, size, 0) {
        this->text = text;
    }

    virtual auto draw() -> void override {
        Widget::draw();

        auto my_theme = get_theme();
        auto color = my_theme->colors.text_color;
        auto text_padding = my_theme->get_padding().get_horizontal();
        auto text_size = my_theme->font->text_size(text);
        auto centered = content.size.centered(text_size, text_padding);
        my_theme->font->write(content, centered, text, color);
    }
};
