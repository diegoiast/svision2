/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <widget.h>
#include <theme.h>

struct Label : public Widget {
    std::string text;

    Label(Position pp, Size size, std::string text) : Widget(pp, size, 0) { this->text = text; }

    virtual auto draw() -> void override {
        auto color = 0;
        this->theme->draw_widget_background(this->content);
        this->content.write_fixed(Position{5, 5}, text, color);
    }
};
