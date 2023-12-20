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
    std::string_view text;

    Label(Position pp, Size size, std::string_view text) : Widget(pp, size, 0) {
        this->text = text;
    }

    virtual auto draw() -> void override {
        Widget::draw();

        auto theme = get_theme();
        auto color = theme->colors.text_color;
        auto text_padding = 5;
        auto text_size = theme->font.text_size(text);
        auto centered = content.size.centered(text_size, text_padding);
        theme->font.write(content, centered, text, color);
    }
};
