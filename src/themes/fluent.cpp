/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "themes/fluent.h"

auto ThemeFluent::get_light_colors(int32_t accent) -> ColorStyle {
    // https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/color
    auto background = MakeColor(243, 243, 243);
    auto normal = MakeColor(229, 229, 229);
    auto disabled = Darker(normal);

    auto colors = ColorStyle();
    colors.window_background = background;

    colors.frame_normal_color1 = normal;
    colors.frame_normal_color2 = colors.frame_normal_color1;
    colors.frame_normal_color3 = colors.frame_normal_color1;
    colors.frame_normal_color4 = colors.frame_normal_color1;

    colors.frame_hover_color1 = colors.frame_normal_color1;
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
    colors.input_background_hover = MakeColor(252, 252, 252);
    colors.input_background_disabled = colors.input_background_normal;
    colors.input_background_selected = accent;

    colors.button_background_1 = MakeColor(251, 251, 251);
    colors.button_background_2 = MakeColor(253, 253, 253);
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
    auto background = !is_default ? colors.button_background_1 : colors.button_selected_background;
    auto color = !is_default ? colors.text_color : colors.button_selected_text;
    auto frame = FrameStyles::Normal;

    switch (state) {
    case ButtonStates::Normal:
        break;
    case ButtonStates::Hovered:
        background = Lighter(background);
        if (!is_default) {
            color = Lighter(colors.text_color);
        }
        break;
    case ButtonStates::ClickedInside:
        if (!is_default) {
            background = colors.window_background;
            color = Darker(colors.frame_normal_color1, 0.6);
        } else {
            background = Lighter(background, 0.20);
        }
        break;
    case ButtonStates::ClickedOutside:
        break;
    }

    draw_frame(content, {0, 0}, content.size, frame,
               is_default ? FrameSize::DoubleFrame : FrameSize::SingleFrame);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);

    content.line(2, content.size.height - 1, content.size.width - 4, content.size.height - 1,
                 Darker(background, is_default ? 0.1 : 0.5));

    auto text_size = font.text_size(text);
    auto centered = content.size.centered(text_size, text_padding);
    font.write(content, centered, text, color);
}

auto ThemeFluent::draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                                ButtonStates state, const std::string &text, CheckboxShape shape,
                                const LayoutParams &padding3) -> void {
    auto checkbox_size = content.size.height;

    // we have 3 colors
    auto background_color = colors.window_background;
    auto checkbox_background = colors.frame_hover_color1;
    auto checkbox_color = colors.text_color;

    switch (state) {
    case ButtonStates::ClickedInside:
        if (is_checked) {
            checkbox_background = Darker(colors.frame_hover_color1);
            checkbox_color = colors.text_selection_color;
        } else {
            checkbox_background = colors.frame_hover_color1;
            checkbox_color = colors.text_selection_color;
        }
        break;
    case ButtonStates::ClickedOutside:
        if (is_checked) {
            checkbox_background = Darker(colors.text_selection_background);
            checkbox_color = colors.text_selection_color;
        } else {
            checkbox_background = colors.frame_normal_color1;
            checkbox_color = colors.text_selection_color;
        }
        break;
    case ButtonStates::Hovered:
        if (is_checked) {
            checkbox_background = colors.text_selection_background;
            checkbox_color = colors.text_selection_color;
        } else {
            checkbox_background = colors.frame_normal_color1;
            checkbox_color = colors.frame_disabled_color1;
            if (!is_enabled) {
                // TODO - disabled color looks wrong
                // checkbox_background = colors.frame_disabled_color1;
                checkbox_color = colors.frame_disabled_color1;
            }
        }
        break;
    case ButtonStates::Normal:
        if (is_checked) {
            checkbox_background = colors.text_selection_background;
            checkbox_color = colors.text_selection_color;
        } else {
            checkbox_background = colors.frame_normal_color1;
            checkbox_color = colors.frame_normal_color1;
            if (!is_enabled) {
                // TODO - disabled color looks wrong
                // checkbox_background = colors.frame_disabled_color1;
                checkbox_color = colors.frame_disabled_color1;
            }
        }
        break;
    }

    content.fill(background_color);

    // this part draws the borders
    {
        auto margin_top = 2;
        auto p = Position{0, 0};
        auto w = Size{checkbox_size, checkbox_size};
        auto m = (checkbox_size) / 2;

        switch (shape) {
        case CheckboxShape::Checkbox:
            if (is_checked)
                content.fill_rounded_rect(p.x, p.y, w.width, w.height, checkbox_background);
            else {
                if (state == ButtonStates::ClickedInside) {
                    content.fill_rect(p.x, p.y, w.width, w.height, checkbox_background);
                } else {
                    content.draw_rounded_rectangle(p.x, p.y, w.width, w.height, 1,
                                                   checkbox_background, checkbox_background);
                }
            }
            break;
        case CheckboxShape::RadioButton:
            content.draw_circle(checkbox_size / 2, checkbox_size / 2, m - margin_top,
                                checkbox_background);
            break;
        }
    }

    // this part draws the checked center
    {
        auto margin_top = 2;
        switch (shape) {
        case CheckboxShape::Checkbox: {
            auto p = Position{margin_top, margin_top};
            auto w = Size{checkbox_size - margin_top * 2, checkbox_size - margin_top * 2};
            if (is_checked) {
                content.line(p.x + 5, p.y + w.height - 10, p.x + 8, p.y + w.height - 5,
                             checkbox_color);
                content.line(p.x + 8, p.y + w.height - 5, p.x + 13, p.y + w.height - 15,
                             checkbox_color);
            } else {
            }
            break;
        }
        case CheckboxShape::RadioButton:
            if (is_checked) {
                auto m2 = checkbox_size / 2;
                auto m3 = checkbox_size / 3.1;
                content.fill_circle(checkbox_size / 2, checkbox_size / 2, m2 - margin_top,
                                    checkbox_background);
                content.fill_circle(checkbox_size / 2, checkbox_size / 2, m3 - margin_top,
                                    checkbox_color);
            }
            break;
        }
    }

    auto text_margin = 5;
    auto text_size = font.text_size(text);
    auto content_rect = content.size;
    auto centered = content_rect.centeredY(text_size);
    centered.x += checkbox_size + text_margin;
    font.write(content, centered, text,
               is_enabled ? colors.text_color : colors.text_color_disabled);
}

auto ThemeFluent::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    // TODO - padding should be the frame size
    auto background = has_focus ? colors.input_background_hover : colors.input_background_normal;

    auto margin_bottom = 1;
    content.fill(background);
    auto frame_size = content.size;
    frame_size.height -= margin_bottom;

    content.line(0, content.size.height - 2, content.size.width, content.size.height - 2,
                 has_focus ? colors.text_selection_background : Darker(background, 0.1));
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
        return {2, 2, 0, 0};
    case PaddingStyle::Label:
        return {5, 5, 5, 5};
    case PaddingStyle::ScrollBar:
        return {12, 12, 12, 12};
    case PaddingStyle::TabHeader:
        return {10, 10, 10, 10};
    }
    return defaultPadding;
}
