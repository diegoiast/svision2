/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "theme.h"

auto ThemeRedmond::draw_window_background(Bitmap &content) -> void {
    content.fill(background_color);
}

auto ThemeRedmond::draw_scrollbar_background(Bitmap &content) -> void {
    auto on_color = ThemeRedmond::line_color1;
    auto off_color = ThemeRedmond::line_color2;
    for (auto y = 1; y < content.size.height - 1; y++) {
        for (auto x = 1; x < content.size.width - 1; x++) {
            auto on = (x + y) % 2 == 0;
            content.put_pixel(x, y, on ? on_color : off_color);
        }
    }
    content.draw_rectangle(0, 0, content.size.width, content.size.height, ThemeRedmond::line_color4,
                           ThemeRedmond::line_color4);
}

auto ThemeRedmond::draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                               ButtonStates state, const std::string &text) -> void {
    auto text_padding = 5;
    auto shadow_padding = text_padding + 1;

    auto background_color = 0;
    auto shadow_offset = 0;
    auto text_offset = 0;

    switch (state) {
    case ButtonStates::Normal:
        if (is_enabled) {
            draw_frame(content, {0, 0}, content.size, true);
        } else {
            draw_frame(content, {0, 0}, content.size, true);
        }
        background_color = ThemeRedmond::background_color;
        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            draw_frame(content, {0, 0}, content.size, true);
        } else {
            draw_frame(content, {0, 0}, content.size, true);
        }
        background_color = ThemeRedmond::background_color_hover;
        break;
    case ButtonStates::ClickedInside:
        if (is_enabled) {
            draw_frame(content, {0, 0}, content.size, false);
        } else {
            draw_frame(content, {0, 0}, content.size, false);
        }
        background_color = ThemeRedmond::background_color_hover;
        shadow_offset = is_default ? 2 : 1;
        break;
    case ButtonStates::ClickedOutside:
        if (is_enabled) {
            draw_frame(content, {0, 0}, content.size, true);
            background_color = ThemeRedmond::background_color;
        } else {
            draw_frame(content, {0, 0}, content.size, false);
            background_color = ThemeRedmond::background_color;
        }
        shadow_offset = 0;
        break;
    default:
        break;
    }

    if (!is_default) {
        content.fill_rect(2, 2, content.size.width - 4, content.size.height - 4, background_color);
    } else {
        auto the_width = content.size.width;
        auto the_height = content.size.height;
        content.draw_rectangle(2, 2, the_width - 4, the_height - 4, line_color4, line_color4);
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

auto ThemeRedmond::draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                                 ButtonStates state, const std::string &text, CheckboxShape shape)
    -> void {
    auto background_color = ThemeRedmond::background_input;
    auto foreground_color = ThemeRedmond::text_color;
    auto checkbox_size = content.size.height;

    switch (state) {
    case ButtonStates::ClickedInside:
        foreground_color = Lighter(foreground_color, 0.2);
        foreground_color = 0x22bb22;
        break;
    case ButtonStates::ClickedOutside:
        foreground_color = Lighter(foreground_color, 0.5);
        foreground_color = 0x228822;
        break;
    case ButtonStates::Hovered:
        if (!is_enabled) {
            foreground_color = ThemeRedmond::text_color_disabled;
        } else {
            foreground_color = Darker(foreground_color, 0.1);
        }
        break;
    case ButtonStates::Normal:
        if (!is_enabled) {
            foreground_color = ThemeRedmond::text_color_disabled;
        }
        break;
    }

    auto m = checkbox_size / 2;
    auto padding = 1;
    content.fill(ThemeRedmond::background_color);

    switch (shape) {
    case CheckboxShape::Checkbox:
        content.fill_rect(padding, padding, checkbox_size - padding * 2,
                          checkbox_size - padding * 2, background_color);
        draw_frame(content, {0, 0}, {checkbox_size, checkbox_size}, false);
        break;
    case CheckboxShape::RadioButton:
        content.fill_circle(m, m, checkbox_size / 2 - padding, background_color);

        if (is_enabled) {
            content.draw_circle(m, m, checkbox_size / 2 - padding, ThemeRedmond::line_color1);
            content.draw_circle(m, m, checkbox_size / 2 - padding - 1, ThemeRedmond::line_color2);
        } else {
            content.draw_circle(m, m, checkbox_size / 2 - padding, ThemeRedmond::text_color_disabled);
        }
        break;
    }

    content.write_fixed({checkbox_size + 5, 5}, text, is_enabled ? foreground_color : ThemeRedmond::text_color_disabled);
    if (is_checked) {
        auto padding = 4;

        switch (shape) {
        case CheckboxShape::Checkbox:
            content.line_thikness(padding, padding, checkbox_size - padding,
                                  checkbox_size - padding, 2, foreground_color);
            content.line_thikness(checkbox_size - padding, padding, padding,
                                  checkbox_size - padding, 2, foreground_color);
            break;
        case CheckboxShape::RadioButton:
            content.fill_circle(m + 1, m + 1, checkbox_size / 2 - padding - 3,
                                ThemeRedmond::line_color2);
            //            content.fill_circle(m+2, m+2, checkbox_size/2 - padding-4,
            //            ThemeRedmond::line_color2);
            break;
        }
    }
}

auto ThemeRedmond::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    draw_frame(content, {0, 0}, content.size, false);
    auto background = has_focus ? ThemeRedmond::background_input : ThemeRedmond::background_color;
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}

auto ThemeRedmond::draw_frame(Bitmap &content, Position position, Size size, bool elevated)
    -> void {
    if (elevated) {
        content.draw_rectangle(0, 0, size.width, size.height, ThemeRedmond::line_color1,
                               ThemeRedmond::line_color2);
        content.draw_rectangle(1, 1, size.width - 2, size.height - 2, ThemeRedmond::line_color3,
                               ThemeRedmond::line_color4);
    } else {
        content.draw_rectangle(0, 0, size.width, size.height, ThemeRedmond::line_color2,
                               ThemeRedmond::line_color1);
        content.draw_rectangle(1, 1, size.width - 2, size.height - 2, ThemeRedmond::line_color4,
                               ThemeRedmond::line_color3);
    }
}

auto ThemeVision::draw_window_background(Bitmap &content) -> void {
    content.fill_rect(0, 0, content.size.width, content.size.height,
                      ThemeVision::window_background_color);
}

auto ThemeVision::draw_scrollbar_background(Bitmap &content) -> void {
    content.fill_rect(1, 1, content.size.width - 1, content.size.height - 1,
                      ThemeVision::window_background_color);
    content.draw_rectangle(0, 0, content.size.width, content.size.height,
                           ThemeVision::button_border, ThemeVision::button_border);
}

auto ThemeVision::draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                              ButtonStates state, const std::string &text) -> void {

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

auto ThemeVision::draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                                ButtonStates state, const std::string &text, CheckboxShape shape)
    -> void {
    auto background_color = ThemeVision::window_background_color;
    auto foreground_color = ThemeVision::text_color;
    auto checkbox_size = content.size.height;
    auto checkbox_color = ThemeVision::button_border_hover;

    switch (state) {
    case ButtonStates::ClickedInside:
        checkbox_color = ThemeVision::button_border_hover;
        is_checked = true;
        break;
    case ButtonStates::ClickedOutside:
        if (is_checked) {
            checkbox_color = ThemeVision::button_border_hover;
        } else {
            // TODO - missing disabled color?
            checkbox_color = ThemeVision::button_border;
        }
        break;
    case ButtonStates::Hovered:
        break;
    case ButtonStates::Normal:
        break;
    }

    content.fill(background_color);
    {
        auto padding = 2;
        auto p = Position{padding, padding};
        auto w = Size{checkbox_size - padding * 2, checkbox_size - padding * 2};
        auto m = checkbox_size / 2;

        switch (shape) {
        case CheckboxShape::Checkbox:
            content.draw_rounded_rectangle(p.x, p.y, w.width, w.height, 1,
                                           ThemeVision::button_border_hover,
                                           ThemeVision::button_border_hover);
            break;
        case CheckboxShape::RadioButton:
            content.draw_circle(m, m, checkbox_size / 2 - padding, is_enabled ? checkbox_color : ThemeVision::text_color_disabled);
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
            auto padding = 5;
            auto m = checkbox_size / 2;
            content.fill_circle(m, m, checkbox_size / 2 - padding, is_enabled ? checkbox_color : ThemeVision::text_color_disabled);
        }
        break;
    }

    content.write_fixed({checkbox_size + 5, 5}, text, is_enabled?  foreground_color : ThemeVision::text_color_disabled );
}

auto ThemeVision::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    auto line1 = ThemeVision::button_border_hover;
    auto line2 = ThemeVision::button_border_hover;
    auto line3 = ThemeVision::button_border_hover;
    auto line4 = ThemeVision::button_border_hover;
    auto background = has_focus ? 0x00FFFFFF : ThemeVision::window_background_color;

    content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
    content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line3, line4);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}

auto ThemePlasma::draw_window_background(Bitmap &content) -> void {
    content.fill_rect(0, 0, content.size.width, content.size.height,
                      ThemePlasma::window_background_color);
    content.line(0, 0, content.size.width - 1, 0, ThemePlasma::border_active);
}

auto ThemePlasma::draw_scrollbar_background(Bitmap &content) -> void {
    auto the_width = content.size.width;
    auto the_height = content.size.height;

    content.fill_rect(0, 0, the_width, the_height, ThemePlasma::window_background_color);
    content.draw_rectangle(0, 0, the_width, the_height - 1, ThemePlasma::border_active,
                           ThemePlasma::border_active);
    content.line(0, the_height - 1, the_width, the_height - 1, ThemePlasma::border_shadow);
}

auto ThemePlasma::draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                              ButtonStates state, const std::string &text) -> void {
    auto text_padding = 5;

    auto background1 = ThemePlasma::button_background_1;
    auto background2 = ThemePlasma::button_background_2;
    auto border = ThemePlasma::border_active;
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
        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            background1 = ThemePlasma::button_background_1;
            background2 = ThemePlasma::button_background_2;
            border = ThemePlasma::border_hover;
        } else {
            border = ThemePlasma::border_disabled;
        }
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
        border = ThemePlasma::border_active;
        color = ThemePlasma::button_selected_text;
        break;
    default:
        break;
    }
    if (!is_enabled) {
        border = ThemePlasma::border_disabled;
    } else {
        if (has_focus)
            border = ThemePlasma::border_hover;
    }

    content.draw_rounded_rectangle(0, 0, content.size.width, content.size.height - 1, 5, border,
                                   border);
    content.line(2, content.size.height - 1, content.size.width - 2, content.size.height - 1,
                 ThemePlasma::border_shadow);

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
    auto the_width = content.size.width;
    auto the_height = content.size.height;
    auto text_position = Position{((the_width - text_size.width) / 2) + text_padding,
                                  ((the_height - text_size.height) / 2) + text_padding};

    if (is_enabled)
        content.write_fixed(text_position, text, color);
}

auto ThemePlasma::draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                                ButtonStates state, const std::string &text, CheckboxShape shape)
    -> void {
    auto background_color = ThemePlasma::window_background_color;
    auto foreground_color = ThemePlasma::text_color;
    auto checkbox_size = content.size.height;
    auto checkbox_color = ThemePlasma::border_hover;
    auto checkbox_border = ThemePlasma::border_disabled;

    switch (state) {
    case ButtonStates::ClickedInside:
        checkbox_color = ThemePlasma::border_disabled;
        checkbox_border = ThemePlasma::border_hover;
        is_checked = true;
        break;
    case ButtonStates::ClickedOutside:
        checkbox_border = ThemePlasma::border_hover;
        checkbox_color = has_focus ? ThemePlasma::border_disabled : ThemePlasma::border_hover;

        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            checkbox_border = ThemePlasma::border_hover;
        }
        break;
    case ButtonStates::Normal:
        checkbox_border = has_focus ? ThemePlasma::border_hover : ThemePlasma::border_disabled;
        checkbox_color = has_focus ? ThemePlasma::border_hover : ThemePlasma::border_disabled;
        break;
    }

    if (!is_enabled) {
        checkbox_color = ThemePlasma::text_color_disabled;
    }

    content.fill(background_color);
    {
        auto padding = 3;
        auto p = Position{padding, padding};
        auto w = Size{checkbox_size - padding * 2, checkbox_size - padding * 2};
        auto m = checkbox_size / 2;

        switch (shape) {
        case CheckboxShape::Checkbox:
            content.draw_rounded_rectangle(p.x, p.y, w.width, w.height, 1, checkbox_border,
                                           checkbox_border);
            break;
        case CheckboxShape::RadioButton:
            if (is_checked) {
                if (is_enabled) {
                    content.fill_circle(m, m, checkbox_size / 2 - padding,
                                        ThemePlasma::button_selected_background);
                    content.draw_circle(m, m, checkbox_size / 2 - padding, ThemePlasma::border_hover);
                } else {
                    content.fill_circle(m, m, checkbox_size / 2 - padding,
                                        checkbox_border);
                    content.draw_circle(m, m, checkbox_size / 2 - padding, ThemePlasma::text_color_disabled);
                }
            } else {
                content.draw_circle(m, m, checkbox_size / 2 - padding, checkbox_border);
            }
            break;
        }
    }

    {
        auto padding = 5;
        auto p = Position{padding, padding};
        auto w = Size{checkbox_size - padding * 2, checkbox_size - padding * 2};
        auto inner_color = is_checked ? checkbox_color : background_color;
        auto m = checkbox_size / 2;

        switch (shape) {
        case CheckboxShape::Checkbox:
            content.fill_rect(p.x, p.y, w.width, w.height, inner_color);
            break;
        case CheckboxShape::RadioButton:
            if (is_checked) {
                content.fill_circle(m, m, 4, ThemePlasma::text_color);
            }
            break;
        }
    }
    content.write_fixed({checkbox_size + 5, 5}, text, is_enabled ? foreground_color : text_color_disabled);
}

auto ThemePlasma::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    auto line1 = has_focus ? ThemePlasma::button_selected_border : ThemePlasma::border_active;
    auto line2 = has_focus ? ThemePlasma::button_selected_border : ThemePlasma::border_active;
    auto line3 = has_focus ? ThemePlasma::button_selected_border : ThemePlasma::border_active;
    auto line4 = has_focus ? ThemePlasma::button_selected_border : ThemePlasma::border_active;
    auto background = 0x00FFFFFF;
    content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
    content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line3, line4);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}
