/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <bitmap.h>
#include <buttonstates.h>

//#define SVISION_OLDIE_ENABLED

struct Theme {
    virtual auto init() -> void = 0;
    virtual auto draw_widget_background(Bitmap &content) -> void = 0;
    virtual auto draw_window_background(Bitmap &content) -> void = 0;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void = 0;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled, ButtonStates state,
                             const std::string &text) -> void = 0;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void = 0;
    virtual auto needs_frame_for_focus() const -> bool = 0;
    virtual auto scrollbar_size() const -> int = 0;
};


// A windows 9x look and feel based theme
struct ThemeRedmond : Theme {
    static const int32_t background_color = 0xc0c0c0;
    static const int32_t background_color_hover = 0xc8c8c8;
    static const int32_t line_color1 = 0xFFFFFF;
    static const int32_t line_color2 = 0x000000;

    static const int32_t line_color3 = 0xFFFFFF;
    static const int32_t line_color4 = 0x808080;

    static const int32_t text_color = 0x000000;

    virtual auto init() -> void override{};
    virtual auto draw_widget_background(Bitmap &content) -> void override {
        content.fill(background_color);
    };
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled, ButtonStates state,
                             const std::string &text) -> void override;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void override;
    virtual auto needs_frame_for_focus() const -> bool override { return true; };
    virtual auto scrollbar_size() const -> int override { return 24; };
protected:
    auto draw_frame(Bitmap &content, Position position, Size size, bool elevated) -> void;
};

struct ThemeVision : Theme {
    static const int32_t window_background_color = MakeColor(240, 240, 240);
    static const int32_t button_background = MakeColor(225, 225, 225);
    static const int32_t button_border = MakeColor(173, 173, 173);
    static const int32_t text_color = MakeColor(0, 0, 0);
    static const int32_t focus_color = MakeColor(80, 80, 80);

    static const int32_t button_border_hover = MakeColor(0, 120, 215);
    static const int32_t button_background_hover = MakeColor(229, 241, 251);
    static const int32_t button_background_click = MakeColor(204, 208, 247);

    virtual auto init() -> void override{};
    virtual auto draw_widget_background(Bitmap &content) -> void override {
        content.fill(window_background_color);
    };
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled, ButtonStates state,
                             const std::string &text) -> void override;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void override;
    virtual auto needs_frame_for_focus() const -> bool override { return false; };
    virtual auto scrollbar_size() const -> int override { return 16; };
};

struct ThemePlasma : Theme {
    static constexpr auto window_background_color = 0xeff0f1;
    static constexpr auto border_active =  0xbbbcbd;
    static constexpr auto border_hover = 0x3daee9;
    static constexpr auto border_disabled = 0xd1d2d3;
    static constexpr auto border_shadow = 0xd1d2d3;

    static const int32_t text_color = 0x2b2e31;
    static const int32_t text_color_disabled = 0x737577;

    static const int32_t button_background_1 = 0xfcfcfc;
    static const int32_t button_background_2 = 0xf5f5f5;
    static const int32_t button_selected_background = 0xd6ecf8;
    static const int32_t button_selected_border = 0x3daee9;
    static const int32_t button_selected_text = 0x31373b;

    virtual auto init() -> void override{};
    virtual auto draw_widget_background(Bitmap &content) -> void override {
        content.fill(window_background_color);
    };
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled, ButtonStates state,
                             const std::string &text) -> void override;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void override;
    virtual auto needs_frame_for_focus() const -> bool override { return false; };
    virtual auto scrollbar_size() const -> int override { return 16; };
};
