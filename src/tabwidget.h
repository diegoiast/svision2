/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stackwidget.h>
#include <tabheader.h>
#include <vector>

struct Button;
struct Stackwidget;
struct HorizontalLayout;

struct TabWidget : Widget {
    std::function<void(TabWidget &, int)> on_page_selected = {};

    TabWidget();

    auto set_active_page(size_t new_index) -> void;
    auto remove_page(size_t position) -> void;
    auto set_buttons(std::shared_ptr<Button> start, std::shared_ptr<Button> end) -> void;

    template <typename T, typename... Args>
    auto add_new_tab(std::string_view name, Args &&...args) -> std::shared_ptr<T> {
        this->needs_redraw = true;
        headers->add_tab(name);
        return stacked_widgets->add_new<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    auto add_new_tab(std::string_view name) -> std::shared_ptr<T> {
        headers->add_tab(name);
        return stacked_widgets->add_new<T>();
    }

    auto get_active_tab() const -> size_t { return headers->get_active_tab(); }

    auto page_names() const -> std::vector<std::string_view> { return headers->names; }

  private:
    std::shared_ptr<HorizontalLayout> top_layout;
    std::shared_ptr<TabHeader> headers;
    std::shared_ptr<Stackwidget> stacked_widgets;
};
