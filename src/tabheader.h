/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <theme.h>
#include <vector>
#include <widget.h>

struct TabHeader : Widget {
    std::vector<std::string_view> names;
    std::function<void(TabHeader &, int)> on_item_selected = {};

    TabHeader();

    auto add_tab(const std::string_view name) -> int;
    auto remove_tab(int index) -> void;
    auto get_tab_string(int index) const -> std::string_view;
    auto set_active_tab(int index) -> void;
    auto get_active_tab() const -> int { return active_tab; }

    virtual auto draw() -> void override;
    virtual auto on_mouse_click(const EventMouse &event) -> EventPropagation override;
    virtual auto on_hover(const EventMouse &event) -> void override;
    virtual auto size_hint() const -> Size override;

  private:
    std::vector<TabHeaderOffsets> tab_offset;

    int active_tab = 0;
    int hover_tab = -1;
};
