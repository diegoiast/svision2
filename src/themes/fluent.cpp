/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "themes/fluent.h"

auto ThemeFluent::draw_widget_background(Bitmap &content, bool has_focus) -> void {
    content.fill(colors.window_background);
}

auto ThemeFluent::draw_window_background(Bitmap &content) -> void {
    content.fill(colors.window_background);
}

auto ThemeFluent::draw_scrollbar_background(Bitmap &content) -> void {
    draw_frame(content, {0, 0}, content.size, FrameStyles::Normal, FrameSize::SingleFrame);
    content.fill_rect(1, 1, content.size.width - 1, content.size.height - 1,
                      colors.window_background);
}

auto ThemeFluent::draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                              ButtonStates state, const std::string &text) -> void {

    auto text_padding = 5;
    auto background = colors.button_background_1;
    auto color = colors.text_color;
    auto frame = FrameStyles::Normal;

    switch (state) {
    case ButtonStates::Normal:
        break;
    case ButtonStates::Hovered:
        frame = FrameStyles::Hover;
        break;
    case ButtonStates::ClickedInside:
        color = colors.button_selected_text;
        background = colors.button_selected_background;
        break;
    case ButtonStates::ClickedOutside:
        break;
    }

    if (has_focus) {
        color = colors.button_selected_text;
        background = colors.button_selected_background;
        draw_frame(content, {0, 0}, content.size, FrameStyles::Hover,
                   is_default ? FrameSize::DoubleFrame : FrameSize::SingleFrame);
    } else {
        draw_frame(content, {0, 0}, content.size, frame,
                   is_default ? FrameSize::DoubleFrame : FrameSize::SingleFrame);
    }
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);

    auto text_size = font.text_size(text);
    auto centered = content.size.centered(text_size, text_padding);
    font.write(content, centered, text, color);
}

auto ThemeFluent::draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                                ButtonStates state, const std::string &text, CheckboxShape shape,
                                const LayoutParams &padding3) -> void {
    auto background_color = colors.window_background;
    auto checkbox_size = content.size.height;
    auto checkbox_color = colors.frame_hover_color1;
    auto padding = 2;

    switch (state) {
    case ButtonStates::ClickedInside:
        checkbox_color = colors.frame_hover_color1;
        is_checked = true;
        break;
    case ButtonStates::ClickedOutside:
        if (is_checked) {
            checkbox_color = colors.frame_hover_color1;
        } else {
            checkbox_color = colors.frame_disabled_color1;
        }
        break;
    case ButtonStates::Hovered:
        break;
    case ButtonStates::Normal:
        break;
    }

    content.fill(background_color);
    {
        auto p = Position{padding, padding};
        auto w = Size{checkbox_size - padding * 2, checkbox_size - padding * 2};
        auto m = checkbox_size / 2;

        switch (shape) {
        case CheckboxShape::Checkbox:
            content.draw_rounded_rectangle(p.x, p.y, w.width, w.height, 1,
                                           colors.frame_hover_color1, colors.frame_hover_color2);
            break;
        case CheckboxShape::RadioButton:
            content.draw_circle(m, m, checkbox_size / 2 - padding,
                                is_enabled ? checkbox_color : colors.text_color_disabled);
            break;
        }
    }

    switch (shape) {
    case CheckboxShape::Checkbox:
        if (is_checked) {
            auto padding = 1;
            auto p = Position{padding, padding};
            auto w = Size{checkbox_size - padding * 2, checkbox_size - padding * 2};
            content.fill_rect(p.x, p.y, w.width, w.height, checkbox_color);
            content.line(p.x + 5, p.y + w.height - 10, p.x + 8, p.y + w.height - 5, 0xffffff);

            content.line(p.x + 8, p.y + w.height - 5, p.x + 13, p.y + w.height - 15, 0xffffff);
        }
        break;
    case CheckboxShape::RadioButton:
        if (is_checked) {
            auto m = checkbox_size / 2;
            content.fill_circle(m, m, checkbox_size / 2 - padding,
                                is_enabled ? checkbox_color : colors.text_color_disabled);
        }
        break;
    }

    auto text_size = font.text_size(text);
    auto content_rect = content.size;
    auto centered = content_rect.centeredY(text_size);
    centered.x += checkbox_size + padding;
    font.write(content, centered, text,
               is_enabled ? colors.text_color : colors.text_color_disabled);
}

auto ThemeFluent::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    // TODO - padding should be the frame size
    auto padding = 1;
    auto background = has_focus ? colors.input_background_hover : colors.input_background_normal;
    content.fill_rect(padding, padding, content.size.width - padding * 2,
                      content.size.height - padding * 2, background);
}

auto ThemeFluent::get_light_colors(int32_t accent) -> ColorStyle {
    // https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/color
    auto background = MakeColor(240, 240, 240);
    auto disabled = MakeColor(130, 130, 130);

    auto colors = ColorStyle();
    colors.window_background = background;

    colors.frame_normal_color1 = MakeColor(173, 173, 173);
    colors.frame_normal_color2 = colors.frame_normal_color1;
    colors.frame_normal_color3 = colors.frame_normal_color1;
    colors.frame_normal_color4 = colors.frame_normal_color1;

    colors.frame_hover_color1 = (accent);
    colors.frame_hover_color2 = colors.frame_hover_color1;
    colors.frame_hover_color3 = colors.frame_hover_color1;
    colors.frame_hover_color4 = colors.frame_hover_color1;

    colors.frame_selected_color1 = (accent);
    colors.frame_selected_color2 = colors.frame_selected_color1;
    colors.frame_selected_color3 = colors.frame_selected_color1;
    colors.frame_selected_color4 = colors.frame_selected_color1;

    colors.frame_disabled_color1 = disabled;
    colors.frame_disabled_color2 = colors.frame_disabled_color1;
    colors.frame_disabled_color3 = colors.frame_disabled_color1;
    colors.frame_disabled_color4 = colors.frame_disabled_color1;

    colors.input_background_normal = MakeColor(255, 255, 255);
    colors.input_background_hover = MakeColor(255, 255, 255);
    colors.input_background_disabled = colors.input_background_normal;
    colors.input_background_selected = accent;

    colors.button_background_1 = MakeColor(225, 225, 225);
    colors.button_background_2 = MakeColor(225, 225, 225);
    colors.button_selected_background = accent;
    colors.button_selected_text = MakeColor(255, 255, 255);

    colors.text_color = MakeColor(0, 0, 0);
    colors.text_color_disabled = disabled;

    colors.text_selection_color = MakeColor(255, 255, 255);
    colors.text_selection_background = accent;
    colors.text_selection_background_hover = Lighter(accent);
    return colors;
}

auto ThemeFluent::get_dark_colors(int32_t accent) -> ColorStyle {
    ColorStyle colors;
    return colors;
}

auto ThemeFluent::draw_listview_background(Bitmap &content, const bool has_focus,
                                           bool draw_background) -> void {

    draw_frame(content, {0, 0}, content.size, has_focus ? FrameStyles::Hover : FrameStyles::Normal,
               FrameSize::SingleFrame);
    if (draw_background) {
        content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2,
                          colors.input_background_normal);
    }
}

void ThemeFluent::draw_listview_item(Bitmap &content, const std::string_view text,
                                     const ItemStatus status, const bool is_hover) {
    auto text_color = status.is_active ? colors.text_selection_color : colors.text_color;
    auto background_color =
        status.is_active ? colors.text_selection_background : colors.input_background_normal;
    if (is_hover && !status.is_active)
        background_color = colors.text_selection_background_hover;
    content.fill(background_color);

    // TODO properly center
    auto text_padding = 5;
    auto text_size = font.text_size(text);
    auto centered = Position{text_padding, text_padding};
    font.write(content, centered, text, text_color);
}

auto ThemeFluent::draw_single_tab(Bitmap &content, const int offset, const bool is_active,
                                  const bool is_hover, const LayoutParams &padding,
                                  const std::string_view name) -> int {
    // TODO
    return 0;
}

LayoutParams ThemeFluent::get_padding(PaddingStyle t) {
    switch (t) {
    case PaddingStyle::Button:
        return {10, 10, 10, 10};
    case PaddingStyle::Checkbox:
        return {5, 5, 5, 5};
    case PaddingStyle::Label:
        return {5, 5, 5, 5};
    case PaddingStyle::ScrollBar:
        return {12, 12, 12, 12};
    case PaddingStyle::TabHeader:
        return {10, 10, 10, 10};
    }
    return defaultPadding;
}
