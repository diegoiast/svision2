/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <widget.h>

struct Stackwidget : Widget {
    Stackwidget() : Stackwidget({}, {}){};
    Stackwidget(Position, Size);

    auto get_current_page() const -> int;
    auto set_current_page(int i) -> void;
    auto get_page(size_t i) -> std::shared_ptr<Widget>;
    auto remove_page(int i) -> void;
    auto get_page_count() const -> size_t;
};
