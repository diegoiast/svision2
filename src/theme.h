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

    int32_t frame_normal_color1 = 0;
    int32_t frame_normal_color2 = 0;
    int32_t frame_normal_color3 = 0;
    int32_t frame_normal_color4 = 0;

    int32_t frame_hover_color1 = 0;
    int32_t frame_hover_color2 = 0;
    int32_t frame_hover_color3 = 0;
    int32_t frame_hover_color4 = 0;

    int32_t frame_selected_color1 = 0;
    int32_t frame_selected_color2 = 0;
    int32_t frame_selected_color3 = 0;
    int32_t frame_selected_color4 = 0;

    int32_t frame_disabled_color1 = 0;
    int32_t frame_disabled_color2 = 0;
    int32_t frame_disabled_color3 = 0;
    int32_t frame_disabled_color4 = 0;

    int32_t input_background_normal = 0;
    int32_t input_background_hover = 0;
    int32_t input_background_selected = 0;
    int32_t input_background_disabled = 0;

    int32_t button_background_1 = 0;
    int32_t button_background_2 = 0;
    int32_t button_selected_background = 0;
    int32_t button_selected_text = 0;

    int32_t text_color = 0;
    int32_t text_color_disabled = 0;

    int32_t text_selection_color = 0;
    int32_t text_selection_background = 0;
};

// I wanted to use "None" - but this is already defined in X11
enum class FrameStyles { NoFrame, Normal, Reversed, Disabled, Hover };
enum class FrameSize { SingleFrame, DoubleFrame, TrippleFrame };

struct Theme {
    ColorStyle colors = {};

    auto draw_frame(Bitmap &content, Position position, Size size, FrameStyles style,
                    FrameSize frame_size) -> void;

    virtual auto init() -> void = 0;
    virtual auto draw_widget_background(Bitmap &content) -> void = 0;
    virtual auto draw_window_background(Bitmap &content) -> void = 0;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void = 0;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             ButtonStates state, const std::string &text) -> void = 0;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string &text, CheckboxShape shape)
        -> void = 0;

    // TODO - missing disabled state
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void = 0;

    virtual auto draw_listview_background(Bitmap &content, const bool has_focus,
                                          bool draw_background) -> void = 0;
    virtual auto draw_listview_item(Bitmap &content, const std::string &text,
                                    const ItemStatus status, bool is_hover) -> void = 0;

    virtual auto needs_frame_for_focus() const -> bool = 0;
    virtual auto scrollbar_size() const -> int = 0;
};

// A windows 9x look and feel based theme
struct ThemeRedmond : Theme {
    static auto get_light_colors() -> ColorStyle;
    static auto get_dark_colors() -> ColorStyle;

    ThemeRedmond() { colors = get_light_colors(); }

    static const int32_t selection_background = 0x2a5faa;
    static const int32_t input_background = MakeColor(0xff, 0xff, 0xff);

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
    virtual auto draw_listview_background(Bitmap &content, const bool has_focus,
                                          bool draw_background) -> void override;
    virtual auto draw_listview_item(Bitmap &content, const std::string &text,
                                    const ItemStatus status, bool is_hover) -> void override;

    virtual auto needs_frame_for_focus() const -> bool override { return true; };
    virtual auto scrollbar_size() const -> int override { return 24; };

};

struct ThemeVision : Theme {
    static constexpr int32_t DefaultAccentLight = 0xff0078d7;
    static auto get_light_colors(int32_t accent = DefaultAccentLight) -> ColorStyle;
    static auto get_dark_colors(int32_t accent = DefaultAccentLight) -> ColorStyle;

    ThemeVision(int32_t accent = DefaultAccentLight) { colors = get_light_colors(accent); }

    static const int32_t selection_background = MakeColor(0, 120, 215);
    static const int32_t selection_foreground = MakeColor(0xff, 0xff, 0xff);
    static const int32_t input_background = MakeColor(0xff, 0xff, 0xff);

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
    virtual auto draw_listview_background(Bitmap &content, const bool has_focus,
                                          bool draw_background) -> void override;
    virtual auto draw_listview_item(Bitmap &content, const std::string &text,
                                    const ItemStatus status, bool is_hover) -> void override;

    virtual auto needs_frame_for_focus() const -> bool override { return false; };
    virtual auto scrollbar_size() const -> int override { return 16; };
};

struct ThemePlasma : Theme {
    static constexpr int32_t DefaultAccentLight = 0x3daee9;
    static auto get_light_colors(int32_t accent = DefaultAccentLight) -> ColorStyle;
    static auto get_dark_colors(int32_t accent = DefaultAccentLight) -> ColorStyle;

    ThemePlasma(int32_t accent = DefaultAccentLight) { colors = get_light_colors(accent); }

    static const int32_t selection_background_hover = 0xe0e0e0;
    static const int32_t selection_background = 0x3daee9;
    static const int32_t input_background = MakeColor(0xff, 0xff, 0xff);

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
    virtual auto draw_listview_background(Bitmap &content, const bool has_focus,
                                          const bool draw_background) -> void override;
    virtual auto draw_listview_item(Bitmap &content, const std::string &text,
                                    const ItemStatus status, bool is_hover) -> void override;

    virtual auto needs_frame_for_focus() const -> bool override { return false; };
    virtual auto scrollbar_size() const -> int override { return 16; };
};
