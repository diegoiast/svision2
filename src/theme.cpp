/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "theme.h"

auto ThemeRedmond::get_light_colors() -> ColorStyle {
    auto colors = ColorStyle();
    colors.window_background = 0xc0c0c0;
    colors.input_background_normal = 0xffffff;
    colors.input_background_hover = 0xffffff; // todo - is this needed at all?
    colors.input_background_disabled = colors.input_background_normal;
    colors.frame_normal_color1 = 0xFFFFFF;
    colors.frame_normal_color2 = 0x808080;
    colors.frame_hover_color1 = 0xFFFFFF;
    colors.frame_hover_color2 = 0x808080;
    colors.frame_disabled_color1 = 0xd1d2d3;
    colors.frame_disabled_color2 = 0xd1d2d3;
    colors.button_background_1 = colors.window_background;
    colors.button_background_2 = colors.window_background;
    colors.button_selected_background = 0xc8c8c8;
    //    colors.button_selected_border = 0x3daee9; // todo - needed?
    colors.button_selected_text = 0xFFFFFF;
    colors.text_color = 0x000000;
    colors.text_color_disabled = 0x606060;
    colors.text_selection_color = 0xFFFFFF;
    colors.text_selection_background = 0x000080;
    return colors;
}

auto ThemeRedmond::get_dark_colors() -> ColorStyle {
    ColorStyle colors;
    return colors;
}

auto ThemeRedmond::draw_window_background(Bitmap &content) -> void {
    content.fill(colors.window_background);
}

auto ThemeRedmond::draw_scrollbar_background(Bitmap &content) -> void {
    auto on_color = colors.frame_normal_color1;
    auto off_color = colors.frame_normal_color2;
    for (auto y = 1; y < content.size.height - 1; y++) {
        for (auto x = 1; x < content.size.width - 1; x++) {
            auto on = (x + y) % 2 == 0;
            content.put_pixel(x, y, on ? on_color : off_color);
        }
    }

    // TODO - seems like we are missing a color in the theme
    auto line_color4 = 0x808080;
    content.draw_rectangle(0, 0, content.size.width, content.size.height, line_color4, line_color4);
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
        background_color = colors.window_background;
        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            draw_frame(content, {0, 0}, content.size, true);
        } else {
            draw_frame(content, {0, 0}, content.size, true);
        }
        // TODO - are we missing a color in the theme?
        //        background_color = ThemeRedmond::background_color_hover;
        background_color = colors.button_selected_background;
        break;
    case ButtonStates::ClickedInside:
        if (is_enabled) {
            draw_frame(content, {0, 0}, content.size, false);
        } else {
            draw_frame(content, {0, 0}, content.size, false);
        }
        // TODO - are we missing a color in the theme?
        //        background_color = ThemeRedmond::background_color_hover;
        background_color = colors.button_selected_background;
        shadow_offset = is_default ? 2 : 1;
        break;
    case ButtonStates::ClickedOutside:
        if (is_enabled) {
            draw_frame(content, {0, 0}, content.size, true);
            background_color = colors.window_background;
        } else {
            draw_frame(content, {0, 0}, content.size, false);
            background_color = colors.window_background;
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
        // TODO - missing color?
        auto line_color4 = colors.frame_hover_color2;
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
    auto background_color = colors.input_background_normal;
    auto foreground_color = colors.text_color;
    auto checkbox_size = content.size.height;

    switch (state) {
    case ButtonStates::ClickedInside:
        // TODO - missing a theme color
        foreground_color = colors.text_color;
        break;
    case ButtonStates::ClickedOutside:
        foreground_color = colors.input_background_hover;
        break;
    case ButtonStates::Hovered:
        if (!is_enabled) {
            foreground_color = colors.text_color_disabled;
        } else {
            foreground_color = colors.text_color;
        }
        break;
    case ButtonStates::Normal:
        if (!is_enabled) {
            foreground_color = colors.text_color_disabled;
        }
        break;
    }

    auto m = checkbox_size / 2;
    auto padding = 1;
    content.fill(colors.window_background);

    switch (shape) {
    case CheckboxShape::Checkbox:
        content.fill_rect(padding, padding, checkbox_size - padding * 2,
                          checkbox_size - padding * 2, background_color);
        draw_frame(content, {0, 0}, {checkbox_size, checkbox_size}, false);
        break;
    case CheckboxShape::RadioButton:
        content.fill_circle(m, m, checkbox_size / 2 - padding, background_color);

        if (is_enabled) {
            content.draw_circle(m, m, checkbox_size / 2 - padding, colors.frame_normal_color1);
            content.draw_circle(m, m, checkbox_size / 2 - padding - 1, colors.frame_normal_color2);
        } else {
            content.draw_circle(m, m, checkbox_size / 2 - padding, colors.frame_disabled_color1);
        }
        break;
    }

    content.write_fixed({checkbox_size + 5, 5}, text,
                        is_enabled ? foreground_color : colors.text_color_disabled);
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
                                colors.frame_normal_color2);
            //            content.fill_circle(m+2, m+2, checkbox_size/2 - padding-4,
            //            ThemeRedmond::line_color2);
            break;
        }
    }
}

auto ThemeRedmond::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    draw_frame(content, {0, 0}, content.size, false);
    auto background = has_focus ? colors.input_background_hover : colors.input_background_disabled;
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}

auto ThemeRedmond::draw_frame(Bitmap &content, Position position, Size size, bool elevated)
    -> void {
    if (elevated) {
        content.draw_rectangle(0, 0, size.width, size.height, colors.frame_normal_color1,
                               colors.frame_normal_color1);

        // missing colors
        content.draw_rectangle(1, 1, size.width - 2, size.height - 2, colors.frame_normal_color2,
                               colors.frame_normal_color1);
    } else {
        content.draw_rectangle(0, 0, size.width, size.height, colors.frame_normal_color1,
                               colors.frame_normal_color1);
        content.draw_rectangle(1, 1, size.width - 2, size.height - 2, colors.frame_normal_color1,
                               colors.frame_normal_color1);
    }
}

auto ThemeVision::get_light_colors() -> ColorStyle {
    ColorStyle colors;
    return colors;
}

auto ThemeVision::get_dark_colors() -> ColorStyle {
    ColorStyle colors;
    return colors;
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

auto ThemePlasma::get_light_colors() -> ColorStyle {
    auto colors = ColorStyle();
    colors.window_background = 0xeff0f1;
    colors.input_background_normal = 0xffffff;
    colors.input_background_hover = 0x0; // todo - is this needed at all?
    colors.input_background_disabled = 0x0;
    colors.frame_normal_color1 = 0xbbbcbd;
    colors.frame_normal_color2 = 0xbbbcbd;
    colors.frame_hover_color1 = 0x3daee9;
    colors.frame_hover_color2 = 0x3daee9;
    colors.frame_disabled_color1 = 0xd1d2d3;
    colors.frame_disabled_color2 = 0xd1d2d3;
    colors.button_background_1 = 0xfcfcfc;
    colors.button_background_2 = 0xf5f5f5;
    colors.button_selected_background = 0xd6ecf8;
    //    colors.button_selected_border = 0x3daee9; // todo - needed?
    colors.button_selected_text = 0x31373b;
    colors.text_color = 0x2b2e31;
    colors.text_color_disabled = 0x737577;
    colors.text_selection_color = 0xffffff;
    colors.text_selection_background = 0x3daee9;
    return colors;
}

auto ThemePlasma::get_dark_colors() -> ColorStyle {
    auto colors = ColorStyle();
    colors.window_background = 0x2ff0f1;
    colors.input_background_normal = 0x393939;
    colors.input_background_hover = 0x0; // todo - is this needed at all?
    colors.input_background_disabled = 0x0;
    colors.frame_normal_color1 = 0xbbbcbd;
    colors.frame_normal_color2 = 0xbbbcbd;
    colors.frame_hover_color1 = 0x3daee9;
    colors.frame_hover_color2 = 0x3daee9;
    colors.frame_disabled_color1 = 0x202020; // todo - is this the correct color?
    colors.frame_disabled_color2 = 0x202020; // todo - is this the correct color?
    colors.button_background_1 = 0xfcfcfc;
    colors.button_background_2 = 0xf5f5f5;
    colors.button_selected_background = 0xd6ecf8;
    //    colors.button_selected_border = 0x3daee9; // todo - needed?
    colors.button_selected_text = 0x31373b;
    colors.text_color = 0x2b2e31;
    colors.text_color_disabled = 0x737577;
    colors.text_selection_color = 0xffffff;
    colors.text_selection_background = 0x3daee9;
    return colors;
}

auto ThemePlasma::draw_window_background(Bitmap &content) -> void {
    content.fill_rect(0, 0, content.size.width, content.size.height, colors.window_background);

    // TODO - port to the theme
    //    content.line(0, 0, content.size.width - 1, 0, ThemePlasma::border_active);
    content.line(0, 0, content.size.width - 1, 0, colors.frame_disabled_color1);
}

auto ThemePlasma::draw_scrollbar_background(Bitmap &content) -> void {
    auto the_width = content.size.width;
    auto the_height = content.size.height;

    content.fill_rect(0, 0, the_width, the_height, colors.window_background);
    content.draw_rectangle(0, 0, the_width, the_height - 1, colors.frame_normal_color1,
                           colors.frame_normal_color1);
    content.line(0, the_height - 1, the_width, the_height - 1, colors.frame_disabled_color1);
}

auto ThemePlasma::draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                              ButtonStates state, const std::string &text) -> void {
    auto text_padding = 5;

    auto background1 = colors.button_background_1;
    auto background2 = colors.button_background_2;
    auto border = colors.frame_normal_color1;
    auto color = colors.text_color;

    switch (state) {
    case ButtonStates::Normal:
        if (has_focus) {
            background1 = colors.button_selected_background;
            background2 = colors.button_selected_background;
        } else {
            background1 = colors.button_background_1;
            background2 = colors.button_background_2;
        }
        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            background1 = colors.button_background_1;
            background2 = colors.button_background_2;
            border = colors.frame_hover_color1;
        } else {
            border = colors.frame_disabled_color1;
        }
        break;
    case ButtonStates::ClickedInside:
        background1 = colors.button_selected_background;
        background2 = colors.button_selected_background;
        border = colors.frame_hover_color1;
        color = colors.button_selected_text;
        break;
    case ButtonStates::ClickedOutside:
        background1 = colors.button_background_1;
        background2 = colors.button_background_2;
        border = colors.frame_hover_color1;
        color = colors.button_selected_text;
        break;
    default:
        break;
    }
    if (!is_enabled) {
        border = colors.frame_disabled_color1;
    } else {
        if (has_focus)
            border = colors.frame_hover_color1;
    }

    content.draw_rounded_rectangle(0, 0, content.size.width, content.size.height - 1, 5, border,
                                   border);

    // TODO - make this into the theme
    auto border_shadow = colors.frame_disabled_color1;
    content.line(2, content.size.height - 1, content.size.width - 2, content.size.height - 1,
                 border_shadow);

    // TODO - widget should be filled with real content from parent
    content.put_pixel(0, content.size.height - 1, colors.window_background);
    content.put_pixel(1, content.size.height - 1, colors.window_background);
    content.put_pixel(content.size.width - 1, content.size.height - 1, colors.window_background);

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
    auto background_color = colors.window_background;
    auto foreground_color = colors.text_color;
    auto checkbox_size = content.size.height;
    auto checkbox_color = colors.frame_hover_color1;
    auto checkbox_border = colors.frame_disabled_color1;

    switch (state) {
    case ButtonStates::ClickedInside:
        checkbox_color = colors.frame_disabled_color1;
        checkbox_border = colors.frame_normal_color1;
        is_checked = true;
        break;
    case ButtonStates::ClickedOutside:
        checkbox_border = colors.frame_hover_color1;
        checkbox_color = has_focus ? colors.frame_disabled_color1 : colors.frame_hover_color1;

        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            checkbox_border = colors.frame_hover_color1;
        }
        break;
    case ButtonStates::Normal:
        checkbox_border = has_focus ? colors.frame_hover_color1 : colors.frame_disabled_color1;
        checkbox_color = has_focus ? colors.frame_hover_color1 : colors.frame_disabled_color1;
        break;
    }

    if (!is_enabled) {
        checkbox_color = colors.text_color_disabled;
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
                                        colors.button_selected_background);
                    content.draw_circle(m, m, checkbox_size / 2 - padding,
                                        colors.frame_hover_color1);
                } else {
                    content.fill_circle(m, m, checkbox_size / 2 - padding, checkbox_border);
                    content.draw_circle(m, m, checkbox_size / 2 - padding,
                                        colors.text_color_disabled);
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
                content.fill_circle(m, m, 4, colors.button_selected_text);
            }
            break;
        }
    }
    content.write_fixed({checkbox_size + 5, 5}, text,
                        is_enabled ? foreground_color : colors.text_color_disabled);
}

auto ThemePlasma::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    auto line1 = has_focus ? colors.frame_hover_color1 : colors.frame_normal_color1;
    auto line2 = has_focus ? colors.frame_hover_color1 : colors.frame_normal_color1;
    auto line3 = has_focus ? colors.frame_hover_color1 : colors.frame_normal_color1;
    auto line4 = has_focus ? colors.frame_hover_color1 : colors.frame_normal_color1;
    auto background = 0x00FFFFFF;
    content.draw_rectangle(0, 0, content.size.width, content.size.height, line1, line2);
    content.draw_rectangle(1, 1, content.size.width - 2, content.size.height - 2, line3, line4);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}
