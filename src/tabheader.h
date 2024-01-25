/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <widget.h>

struct TabHeader : Widget {
    std::vector<std::string_view> names;

    TabHeader();

    auto add_tab(const std::string_view name) -> int;
    auto remove_tab(int index) -> void;
    auto get_tab_string(int index) const -> std::string_view;
    auto set_active_tab(int index) -> void;
    auto get_active_tab() const -> int { return active_tab; }

    virtual auto draw() -> void override;
    virtual auto size_hint() const -> Size override;

  private:
    int active_tab = 0;
};
