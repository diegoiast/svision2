/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <bitmap.h>
#include <buttonstates.h>
#include <checkboxshape.h>

struct ColorStyle {
    int32_t window_background = 0;
    int32_t input_background_normal = 0;
    int32_t input_background_hover = 0;
    int32_t input_background_disabled = 0;
    int32_t frame_normal_color1 = 0;
    int32_t frame_normal_color2 = 0;
    int32_t frame_hover_color1 = 0;
    int32_t frame_hover_color2 = 0;
    int32_t frame_disabled_color1 = 0;
    int32_t frame_disabled_color2 = 0;
    int32_t button_background_1 = 0;
    int32_t button_background_2 = 0;
    int32_t button_selected_background = 0;
    int32_t button_selected_text = 0;
    int32_t text_color = 0;
    int32_t text_color_disabled = 0;
    int32_t text_selection_color = 0;
    int32_t text_selection_background = 0;
};

struct Theme {
    ColorStyle colors = {};

    virtual auto init() -> void = 0;
    virtual auto draw_widget_background(Bitmap &content) -> void = 0;
    virtual auto draw_window_background(Bitmap &content) -> void = 0;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void = 0;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             ButtonStates state, const std::string &text) -> void = 0;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string &text, CheckboxShape shape)
        -> void = 0;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void = 0;
    virtual auto needs_frame_for_focus() const -> bool = 0;
    virtual auto scrollbar_size() const -> int = 0;
};

// A windows 9x look and feel based theme
struct ThemeRedmond : Theme {
    static const int32_t background_color = 0xc0c0c0;
    static const int32_t background_color_hover = 0xc8c8c8;
    static const int32_t background_input = 0xFFFFFF;
    static const int32_t line_color1 = 0xFFFFFF;
    static const int32_t line_color2 = 0x000000;

    static const int32_t line_color3 = 0xFFFFFF;
    static const int32_t line_color4 = 0x808080;

    static const int32_t text_color = 0x000000;
    static const int32_t text_color_disabled = 0x606060;

    virtual auto init() -> void override{};
    virtual auto draw_widget_background(Bitmap &content) -> void override {
        content.fill(background_color);
    };
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             ButtonStates state, const std::string &text) -> void override;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string &text, CheckboxShape shape)
        -> void override;
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
    static const int32_t text_color_disabled = MakeColor(130,130, 130);
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
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             ButtonStates state, const std::string &text) -> void override;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string &text, CheckboxShape shape)
        -> void override;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void override;
    virtual auto needs_frame_for_focus() const -> bool override { return false; };
    virtual auto scrollbar_size() const -> int override { return 16; };
};

struct ThemePlasma : Theme {
    static auto get_light_colors() -> ColorStyle;
    static auto get_dark_colors() -> ColorStyle;

    ThemePlasma() { colors = get_light_colors(); }

    virtual auto init() -> void override{};
    virtual auto draw_widget_background(Bitmap &content) -> void override {
        content.fill(colors.window_background);
    };
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             ButtonStates state, const std::string &text) -> void override;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string &text, CheckboxShape shape)
        -> void override;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void override;
    virtual auto needs_frame_for_focus() const -> bool override { return false; };
    virtual auto scrollbar_size() const -> int override { return 16; };
};
