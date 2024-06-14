/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>
#include <string_view>
#include <vector>

#include <checkboxshape.h>
#include <widget.h>

struct ScrollBar;

struct ItemAdapter {
    // ALA TurboVision
    using PWidget = std::shared_ptr<Widget>;
    virtual ~ItemAdapter() = default;

    virtual auto get_count() const -> size_t = 0;
    virtual auto get_widget(size_t position, std::shared_ptr<Theme> theme) -> PWidget = 0;
    virtual auto set_content(PWidget widget, size_t position, ItemStatus status) -> void = 0;
};

struct ListItemAdapter : ItemAdapter {
    std::vector<std::string> strings;

    explicit ListItemAdapter(const std::vector<std::string> &s) { this->strings = s; };
    virtual auto get_count() const -> size_t override { return strings.size(); }
    virtual auto get_widget(size_t position, std::shared_ptr<Theme> theme) -> PWidget override;
    virtual auto set_content(PWidget widget, size_t position, ItemStatus status) -> void override;
};

struct ListItemWidget : public Widget {
    std::string_view text = {};
    ItemStatus status = {};

    ListItemWidget(Position pp, Size size, std::string_view text) : Widget(pp, size, 0) {
        this->text = text;
    }

    virtual auto draw() -> void override;
};

struct ListView : public Widget {
    enum class SelectionReason {
        Mouse,
        Keyboard,
        KeyboardMove,
    };

    std::shared_ptr<ScrollBar> scrollbar = {};
    std::shared_ptr<ItemAdapter> adapter = {};
    std::vector<std::shared_ptr<Widget>> reserved_widgets = {};
    std::function<void(ListView &, int, SelectionReason)> on_item_selected;
    int current_item = 0;

    ListView();
    ListView(Position position, Size size);
    virtual auto draw() -> void override;
    virtual auto on_mouse_click(const EventMouse &event) -> EventPropagation override;
    virtual auto on_keyboard(const EventKeyboard &) -> EventPropagation override;
    virtual auto on_resize() -> void override;

    auto did_adapter_update() -> void;
};
