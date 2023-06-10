/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "theme.h"

#if defined(SVISION_OLDIE_ENABLED)
auto ThemeOldie::draw_window_background(Bitmap &content) -> void { content.fill(background_color); }

auto ThemeOldie::draw_button(Bitmap &content, bool has_focus, bool is_default, ButtonStates state,
                             const std::string &text) -> void {
    auto text_padding = 5;
    auto shadow_padding = text_padding + 1;

    auto background_color = background_color_normal;
    auto line1 = line_color1;
    auto line2 = line_color2;
    auto shadow_offset = 0;
    auto text_offset = 0;

    switch (state) {
    case ButtonStates::Normal:
        line1 = line_color1;
        line2 = line_color2;
        break;
    case ButtonStates::Hovered:
        line1 = MakeColor(0x00, 0xff, 0x00);
        line2 = MakeColor(0x00, 0xff, 0x00);
        background_color = background_color_hover;
        break;
    case ButtonStates::ClickedInside:
        line1 = line_color2;
        line2 = line_color1;
        background_color = background_color_hover;
        shadow_offset = 1;
        break;
    case ButtonStates::ClickedOutside:
        line1 = line_color1;
        line2 = line_color2;
        background_color = background_color_hover;
        shadow_offset = 0;
        break;

    default:
        break;
    }

    content.line(0, 0, 0, content.size.height - 1, line1);
    content.line(0, 0, content.size.width - 1, 0, line1);
    content.line(content.size.width - 1, 0, content.size.width - 1, content.size.height - 1, line2);
    content.line(0, content.size.height - 1, content.size.width - 1, content.size.height - 1,
                 line2);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background_color);
    content.write_fixed(Position{shadow_padding + shadow_offset, shadow_padding + shadow_offset},
                        text, 0);
    content.write_fixed(Position{text_padding + text_offset, text_padding + text_offset}, text,
                        0xffffff);
}
#endif

auto ThemeRedmond::draw_widget_frame(Bitmap &content, bool selected, bool active) -> void {
    auto line1 = ThemeRedmond::line_color2;
    auto line2 = ThemeRedmond::line_color1;
    auto line3 = ThemeRedmond::line_color4;
    auto line4 = ThemeRedmond::line_color3;
    if (selected) {
        content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
        content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line3, line4);
        content.fill_rect(2, 2, content.size.width - 4, content.size.height - 4, 0xffffff);
    } else {
        content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
        content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, 0xf0f0f0);
    }
}

auto ThemeRedmond::draw_window_background(Bitmap &content) -> void {
    content.fill(background_color);
}

auto ThemeRedmond::draw_button(Bitmap &content, bool has_focus, bool is_default, ButtonStates state,
                               const std::string &text) -> void {
    auto text_padding = 5;
    auto shadow_padding = text_padding + 1;

    auto background_color = 0;
    auto line1 = ThemeRedmond::line_color1;
    auto line2 = ThemeRedmond::line_color1;
    auto line3 = ThemeRedmond::line_color1;
    auto line4 = ThemeRedmond::line_color1;
    auto shadow_offset = 0;
    auto text_offset = 0;

    switch (state) {
    case ButtonStates::Normal:
        line1 = ThemeRedmond::line_color1;
        line2 = ThemeRedmond::line_color2;
        line3 = ThemeRedmond::line_color3;
        line4 = ThemeRedmond::line_color4;
        background_color = ThemeRedmond::background_color;
        break;
    case ButtonStates::Hovered:
        line1 = ThemeRedmond::line_color1;
        line2 = ThemeRedmond::line_color2;
        line3 = ThemeRedmond::line_color3;
        line4 = ThemeRedmond::line_color4;
        background_color = ThemeRedmond::background_color_hover;
        break;
    case ButtonStates::ClickedInside:
        line1 = ThemeRedmond::line_color2;
        line2 = ThemeRedmond::line_color1;
        line3 = ThemeRedmond::line_color4;
        line4 = ThemeRedmond::line_color3;
        background_color = ThemeRedmond::background_color_hover;
        shadow_offset = is_default ? 2 : 1;
        break;
    case ButtonStates::ClickedOutside:
        line1 = ThemeRedmond::line_color1;
        line2 = ThemeRedmond::line_color2;
        line3 = ThemeRedmond::line_color3;
        line4 = ThemeRedmond::line_color4;
        background_color = ThemeRedmond::background_color;
        shadow_offset = 0;
        break;

    default:
        break;
    }

    content.fill_rect(0, 0, content.size.width, content.size.height, 0xffaabb);
    if (!is_default) {
        content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
        content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line3, line4);
        content.fill_rect(2, 2, content.size.width - 4, content.size.height - 4, background_color);
    } else {
        content.draw_rectangle(0, 0, content.size.width, content.size.height, line2, line2);
        content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line1, line2);
        content.draw_rectangle(2, 2, content.size.width - 4, content.size.height - 4, line3, line4);
        content.fill_rect(3, 3, content.size.width - 6, content.size.height - 6, background_color);
    }

    auto text_size = Bitmap::text_size(text) + (std::max(text_padding, shadow_padding) +
                                                std::max(text_offset, shadow_offset));
    auto text_position = Position{((content.size.width - text_size.width) / 2) + text_padding,
                                  ((content.size.height - text_size.height) / 2) + text_padding};
    auto shadow_position =
        Position{((content.size.width - text_size.width) / 2) + shadow_padding,
                 ((content.size.height - text_size.height) / 2) + shadow_padding};
    content.write_fixed(shadow_position, text, 0x0);
    content.write_fixed(text_position, text, 0xffffff);
}

auto ThemeRedmond::draw_input_background(Bitmap &content, const bool has_focus) -> void
{
    auto line1 = ThemeRedmond::line_color2;
    auto line2 = ThemeRedmond::line_color1;
    auto line3 = ThemeRedmond::line_color4;
    auto line4 = ThemeRedmond::line_color3;
    auto background = has_focus ? 0x00FFFFFF : ThemeRedmond::background_color;
    content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
    content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line3, line4);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}


auto ThemeVision::draw_widget_frame(Bitmap &content, bool selected, bool active) -> void {
    auto color = ThemeVision::button_border;
    if (selected) {
        color = ThemeVision::button_border_hover;
    }
    content.draw_rounded_rectangle(0, 0, content.size.width, content.size.height, 5, color, color);
}

auto ThemeVision::draw_window_background(Bitmap &content) -> void {
    content.fill_rect(0, 0, content.size.width, content.size.height,
                      ThemeVision::window_background_color);
}

auto ThemeVision::draw_button(Bitmap &content, bool has_focus, bool is_default, ButtonStates state,
                              const std::string &text) -> void {

    auto text_padding = 5;

    auto background = ThemeVision::button_background;
    auto border = ThemeVision::button_border;
    auto color = ThemeVision::text_color;
    auto double_border = false;

    switch (state) {
    case ButtonStates::Normal:
        border = ThemeVision::button_border;
        if (has_focus) {
            background = ThemeVision::button_background_hover;
        } else {
            background = ThemeVision::button_background;
            if (is_default) {
                border = ThemeVision::button_border_hover;
            }
        }
        break;
    case ButtonStates::Hovered:
        background = ThemeVision::button_background_hover;
        border = ThemeVision::button_border_hover;
        break;
    case ButtonStates::ClickedInside:
        background = ThemeVision::button_background_click;
        border = ThemeVision::button_border_hover;
        break;
    case ButtonStates::ClickedOutside:
        background = ThemeVision::button_background;
        border = ThemeVision::button_border_hover;
        break;
    default:
        break;
    }

    double_border |= is_default;
    if (has_focus) {
        double_border = false;
    }

    content.fill(0xff0000);
    content.draw_rectangle(0, 0, content.size.width, content.size.height, border, border);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
    if (double_border) {
        content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, border,
                               border);
    }
    if (has_focus) {
        auto padding = 3;
        content.draw_rectangle(padding, padding, content.size.width - padding * 2,
                               content.size.height - padding * 2, ThemeVision::focus_color,
                               ThemeVision::focus_color);
    }

    auto text_size = Bitmap::text_size(text) + text_padding;
    auto text_position = Position{((content.size.width - text_size.width) / 2) + text_padding,
                                  ((content.size.height - text_size.height) / 2) + text_padding};

    content.write_fixed(text_position, text, color);
}

auto ThemeVision::draw_input_background(Bitmap &content, const bool has_focus) -> void
{
    auto line1 = ThemeVision::button_border_hover;
    auto line2 = ThemeVision::button_border_hover;
    auto line3 = ThemeVision::button_border_hover;
    auto line4 = ThemeVision::button_border_hover;
    auto background = has_focus ? 0x00FFFFFF : ThemeVision::window_background_color;;
    content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
    content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line3, line4);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}

auto ThemePlasma::draw_widget_frame(Bitmap &content, bool selected, bool active) -> void {
    auto color = ThemePlasma::button_border;
    if (selected) {
        color = ThemePlasma::button_selected_border;
    }
    content.draw_rounded_rectangle(0, 0, content.size.width, content.size.height, 5, color, color);
}

auto ThemePlasma::draw_window_background(Bitmap &content) -> void {
    content.fill_rect(0, 0, content.size.width, content.size.height,
                      ThemePlasma::window_background_color);
    content.line(0, 0, content.size.width - 1, 0, button_border);
}

auto ThemePlasma::draw_button(Bitmap &content, bool has_focus, bool is_default, ButtonStates state,
                              const std::string &text) -> void {
    auto text_padding = 5;

    auto background1 = ThemePlasma::button_background_1;
    auto background2 = ThemePlasma::button_background_2;
    auto border = ThemePlasma::button_border;
    auto color = ThemePlasma::text_color;

    switch (state) {
    case ButtonStates::Normal:
        if (has_focus) {
            background1 = ThemePlasma::button_selected_background;
            background2 = ThemePlasma::button_selected_background;
        } else {
            background1 = ThemePlasma::button_background_1;
            background2 = ThemePlasma::button_background_2;
        }
        border = button_border;
        break;
    case ButtonStates::Hovered:
        background1 = ThemePlasma::button_background_1;
        background2 = ThemePlasma::button_background_2;
        border = button_selected_border;
        break;
    case ButtonStates::ClickedInside:
        background1 = ThemePlasma::button_selected_background;
        background2 = ThemePlasma::button_selected_background;
        border = ThemePlasma::button_selected_border;
        color = ThemePlasma::button_selected_text;
        break;
    case ButtonStates::ClickedOutside:
        background1 = ThemePlasma::button_background_1;
        background2 = ThemePlasma::button_background_2;
        border = ThemePlasma::button_border;
        color = ThemePlasma::button_selected_text;
        break;
    default:
        break;
    }

    content.draw_rounded_rectangle(0, 0, content.size.width, content.size.height - 1, 5, border,
                                   border);
    content.line(2, content.size.height - 1, content.size.width - 2, content.size.height - 1,
                 button_shadow);

    // TODO - widget should be filled with real content from parent
    content.put_pixel(0, content.size.height - 1, ThemePlasma::window_background_color);
    content.put_pixel(1, content.size.height - 1, ThemePlasma::window_background_color);
    content.put_pixel(content.size.width - 1, content.size.height - 1,
                      ThemePlasma::window_background_color);

    if (background1 == background2) {
        content.fill_rect(1, 1, content.size.width - 2, content.size.height - 3, background1);
    } else {
        content.fill_rect_gradient(1, 1, content.size.width - 2, content.size.height - 3,
                                   background1, background2);
    }

    auto text_size = Bitmap::text_size(text) + text_padding;
    auto text_position = Position{((content.size.width - text_size.width) / 2) + text_padding,
                                  ((content.size.height - text_size.height) / 2) + text_padding};

    content.write_fixed(text_position, text, color);
}

auto ThemePlasma::draw_input_background(Bitmap &content, const bool has_focus) -> void
{
    auto line1 = has_focus ? button_selected_border : ThemePlasma::button_border;
    auto line2 = has_focus ? button_selected_border : ThemePlasma::button_border;
    auto line3 = has_focus ? button_selected_border : ThemePlasma::button_border;
    auto line4 = has_focus ? button_selected_border : ThemePlasma::button_border;
    auto background = 0x00FFFFFF;
    content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
    content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line3, line4);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}
