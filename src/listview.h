/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <vector>
#include <widget.h>

class ScrollBar;

struct ItemStatus {
    bool is_selected = false;
    bool is_active = false;
};

struct ItemAdapter {
    // ALA TurboVision
    using PWidget = std::shared_ptr<Widget>;

    virtual auto get_count() -> size_t const = 0;
    virtual auto get_widget(size_t position) -> PWidget = 0;
    virtual auto set_content(PWidget widget, size_t position, ItemStatus status) -> void = 0;
};

struct ListItemAdapter : ItemAdapter {
    std::vector<std::string> strings;

    ListItemAdapter(std::vector<std::string> s) { this->strings = s; };
    virtual auto get_count() -> size_t const override { return strings.size(); }
    virtual auto get_widget(size_t position) -> PWidget override;
    virtual auto set_content(PWidget widget, size_t position, ItemStatus status) -> void override;
};

struct ListView : public Widget {
    using LPWidget = std::list<std::shared_ptr<Widget>>;

    std::shared_ptr<ScrollBar> scrollbar = {};
    std::shared_ptr<ItemAdapter> adapter = {};
    std::vector<std::shared_ptr<Widget>> reserved_widgets = {};
    size_t offset = 0;

    ListView(Position position, Size size);
    virtual auto draw() -> void override;

    auto did_adapter_update() -> void;
};
