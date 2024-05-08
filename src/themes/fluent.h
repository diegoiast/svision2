/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

// Theme reference is found here (we try to match as possible look and feel):
// https://developer.microsoft.com/en-us/fluentui?fabricVer=6#/controls/web/button
// https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/color

#include <theme.h>

struct ThemeFluent : Theme {
    static constexpr int32_t DefaultAccentLight = 0xff1975c5;
    static auto get_light_colors(int32_t accent = DefaultAccentLight) -> ColorStyle;
    static auto get_dark_colors(int32_t accent = DefaultAccentLight) -> ColorStyle;

    explicit ThemeFluent(FontProvider &f, int32_t accent = DefaultAccentLight) : Theme(f) {
        colors = get_light_colors(accent);
    }

    virtual auto init() -> void override{};
    virtual auto draw_widget_background(Bitmap &content, bool has_focus) -> void override;
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             bool has_frame, ButtonStates state, const std::string_view text)
        -> void override;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string_view text, CheckboxShape shape,
                               const LayoutParams &padding) -> void override;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void override;
    virtual auto draw_listview_background(Bitmap &content, const bool has_focus,
                                          bool draw_background) -> void override;
    virtual auto draw_listview_item(Bitmap &content, const std::string_view text,
                                    const ItemStatus status, const bool is_hover) -> void override;
    virtual auto draw_single_tab(Bitmap &content, const int offset, const bool is_active,
                                 const bool is_hover, const LayoutParams &padding,
                                 const std::string_view name) -> int override;

    virtual auto needs_frame_for_focus() const -> bool override { return false; };
    virtual auto scrollbar_size() const -> int override { return 16; };
    virtual auto get_padding(PaddingStyle t = PaddingStyle::Label) -> LayoutParams override;
};
