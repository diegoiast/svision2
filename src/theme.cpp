/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "theme.h"

auto Theme::draw_frame(Bitmap &content, Position position, Size size, FrameStyles style,
                       FrameSize frame_size) -> void {
    switch (style) {
    case FrameStyles::Normal:
        switch (frame_size) {
        case FrameSize::TrippleFrame:
            content.draw_rectangle(2, 2, size.width - 4, size.height - 4,
                                   colors.frame_normal_color4, colors.frame_normal_color4);
        case FrameSize::DoubleFrame:
            content.draw_rectangle(1, 1, size.width - 2, size.height - 2,
                                   colors.frame_normal_color3, colors.frame_normal_color4);
        case FrameSize::SingleFrame:
            content.draw_rectangle(position.x, position.y, size.width, size.height,
                                   colors.frame_normal_color1, colors.frame_normal_color2);
        default:
            break;
        }
        break;
    case FrameStyles::Reversed:
        content.draw_rectangle(position.x, position.y, size.width, size.height,
                               colors.frame_normal_color2, colors.frame_normal_color1);
        content.draw_rectangle(1, 1, size.width - 2, size.height - 2, colors.frame_normal_color1,
                               colors.frame_normal_color2);
        break;
    case FrameStyles::Disabled:
        content.draw_rectangle(position.x, position.y, size.width, size.height,
                               colors.frame_disabled_color1, colors.frame_disabled_color2);
        content.draw_rectangle(1, 1, size.width - 2, size.height - 2, colors.frame_disabled_color3,
                               colors.frame_disabled_color4);
        break;
    case FrameStyles::Hover:
        content.draw_rectangle(position.x, position.y, size.width, size.height,
                               colors.frame_hover_color1, colors.frame_hover_color2);
        content.draw_rectangle(1, 1, size.width - 2, size.height - 2, colors.frame_hover_color3,
                               colors.frame_hover_color4);
        break;
    case FrameStyles::NoFrame:
        break;
    }
}

auto ThemeRedmond::get_light_colors() -> ColorStyle {
    auto colors = ColorStyle();
    colors.window_background = 0xc0c0c0;

    colors.frame_normal_color1 = 0xFFFFFF;
    colors.frame_normal_color2 = 0x000000;
    colors.frame_normal_color3 = 0xFFFFFF;
    colors.frame_normal_color4 = 0x808080;

    colors.frame_hover_color1 = colors.frame_normal_color1;
    colors.frame_hover_color2 = colors.frame_normal_color2;
    colors.frame_hover_color3 = colors.frame_normal_color3;
    colors.frame_hover_color4 = colors.frame_normal_color4;

    colors.frame_selected_color1 = colors.frame_normal_color1;
    colors.frame_selected_color2 = colors.frame_normal_color2;
    colors.frame_selected_color3 = colors.frame_normal_color3;
    colors.frame_selected_color4 = colors.frame_normal_color4;

    colors.frame_disabled_color1 = 0xd1d2d3;
    colors.frame_disabled_color2 = 0xd1d2d3;
    colors.frame_disabled_color3 = 0xd1d2d3;
    colors.frame_disabled_color4 = 0xd1d2d3;

    colors.input_background_normal = 0xffffff;
    colors.input_background_hover = colors.input_background_normal;
    colors.input_background_disabled = colors.window_background;
    colors.input_background_selected = colors.input_background_normal;

    colors.button_background_1 = colors.window_background;
    colors.button_background_2 = colors.window_background;
    colors.button_selected_background = 0xc8c8c8;
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
    auto topleft = Position{0, 0};
    auto frame_size = is_default ? FrameSize::TrippleFrame : FrameSize::DoubleFrame;

    switch (state) {
    case ButtonStates::Normal:
        if (is_enabled) {
            draw_frame(content, topleft, content.size, FrameStyles::Normal, frame_size);
        } else {
            draw_frame(content, topleft, content.size, FrameStyles::Disabled, frame_size);
        }
        background_color = colors.window_background;
        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            draw_frame(content, topleft, content.size, FrameStyles::Hover, frame_size);
        } else {
            draw_frame(content, topleft, content.size, FrameStyles::Disabled, frame_size);
        }
        // TODO - are we missing a color in the theme?
        background_color = colors.button_selected_background;
        break;
    case ButtonStates::ClickedInside:
        if (is_enabled) {
            draw_frame(content, topleft, content.size, FrameStyles::Reversed, frame_size);
        } else {
            draw_frame(content, topleft, content.size, FrameStyles::Disabled, frame_size);
        }
        background_color = colors.button_background_1;
        shadow_offset = is_default ? 2 : 1;
        break;
    case ButtonStates::ClickedOutside:
        background_color = colors.window_background;
        if (is_enabled) {
            draw_frame(content, topleft, content.size, FrameStyles::Normal, frame_size);
        } else {
            draw_frame(content, topleft, content.size, FrameStyles::Disabled, frame_size);
        }
        shadow_offset = 0;
        break;
    default:
        break;
    }

    if (!is_default) {
        content.fill_rect(2, 2, content.size.width - 4, content.size.height - 4, background_color);
    } else {
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
        draw_frame(content, {0, 0}, {checkbox_size, checkbox_size}, FrameStyles::Normal,
                   FrameSize::SingleFrame);
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
    draw_frame(content, {0, 0}, content.size, FrameStyles::Reversed, FrameSize::SingleFrame);
    auto background = has_focus ? colors.input_background_selected : colors.input_background_normal;
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}

auto ThemeVision::get_light_colors(int32_t accent) -> ColorStyle {
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

    colors.input_background_normal = MakeColor(245, 245, 245);
    colors.input_background_hover = MakeColor(255, 255, 255);
    colors.input_background_disabled = colors.input_background_normal;
    colors.input_background_selected = colors.input_background_normal;

    colors.button_background_1 = MakeColor(225, 225, 225);
    colors.button_background_2 = MakeColor(225, 225, 225);
    colors.button_selected_background = accent;
    colors.button_selected_text = MakeColor(255, 255, 255);

    colors.text_color = MakeColor(0, 0, 0);
    colors.text_color_disabled = disabled;

    colors.text_selection_color = MakeColor(255, 255, 255);
    colors.text_selection_background = accent;
    return colors;
}

auto ThemeVision::get_dark_colors(int32_t accent) -> ColorStyle {
    ColorStyle colors;
    return colors;
}

auto ThemeRedmond::draw_listview_background(Bitmap &content, const bool has_focus,
                                            bool draw_background) -> void {
    draw_frame(content, {0, 0}, content.size, has_focus ? FrameStyles::Hover : FrameStyles::Normal,
               FrameSize::SingleFrame);
    auto background = has_focus ? colors.input_background_selected : colors.input_background_normal;
    if (draw_background) {
        content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
    }
}

auto ThemeVision::draw_window_background(Bitmap &content) -> void {
    content.fill_rect(0, 0, content.size.width, content.size.height, colors.window_background);
}

auto ThemeVision::draw_scrollbar_background(Bitmap &content) -> void {
    draw_frame(content, {0, 0}, content.size, FrameStyles::Normal, FrameSize::SingleFrame);
    content.fill_rect(1, 1, content.size.width - 1, content.size.height - 1,
                      colors.window_background);
}

auto ThemeVision::draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
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

    auto text_size = Bitmap::text_size(text) + text_padding;
    auto text_position = Position{((content.size.width - text_size.width) / 2) + text_padding,
                                  ((content.size.height - text_size.height) / 2) + text_padding};

    content.write_fixed(text_position, text, color);
}

auto ThemeVision::draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                                ButtonStates state, const std::string &text, CheckboxShape shape)
    -> void {
    auto background_color = colors.window_background;
    auto checkbox_size = content.size.height;
    auto checkbox_color = colors.frame_hover_color1;

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
        auto padding = 2;
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
            auto padding = 5;
            auto m = checkbox_size / 2;
            content.fill_circle(m, m, checkbox_size / 2 - padding,
                                is_enabled ? checkbox_color : colors.text_color_disabled);
        }
        break;
    }

    content.write_fixed({checkbox_size + 5, 5}, text,
                        is_enabled ? colors.text_color : colors.text_color_disabled);
}

auto ThemeVision::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    auto background = has_focus ? colors.input_background_hover : colors.input_background_normal;
    auto frame = has_focus ? FrameStyles::Hover : FrameStyles::Normal;
    draw_frame(content, {0, 0}, content.size, frame, FrameSize::SingleFrame);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
}

// static constexpr int32_t DefaultAccentLight = 0x3daee9;

auto ThemePlasma::get_light_colors(int32_t accent) -> ColorStyle {
    auto background = 0xeff0f1;
    auto disabled = 0x737577;

    auto colors = ColorStyle();
    colors.window_background = background;

    colors.frame_normal_color1 = 0xbbbcbd;
    colors.frame_normal_color2 = colors.frame_normal_color1;
    colors.frame_normal_color3 = colors.frame_normal_color1;
    colors.frame_normal_color4 = colors.frame_normal_color1;

    colors.frame_hover_color1 = accent;
    colors.frame_hover_color2 = colors.frame_hover_color1;
    colors.frame_hover_color3 = colors.frame_hover_color1;
    colors.frame_hover_color4 = colors.frame_hover_color1;

    colors.frame_selected_color1 = accent;
    colors.frame_selected_color2 = colors.frame_selected_color1;
    colors.frame_selected_color3 = colors.frame_selected_color1;
    colors.frame_selected_color4 = colors.frame_selected_color1;

    colors.frame_disabled_color1 = disabled;
    colors.frame_disabled_color2 = colors.frame_disabled_color1;
    colors.frame_disabled_color3 = colors.frame_disabled_color1;
    colors.frame_disabled_color4 = colors.frame_disabled_color1;

    colors.input_background_normal = Darker(0xffffff, 0.002);
    colors.input_background_hover = 0xffffff;
    colors.input_background_disabled = disabled;
    colors.input_background_selected = 0xffffff;

    colors.button_background_1 = Lighter(background);
    colors.button_background_2 = Lighter(background, 0.05);
    colors.button_selected_background = Darker(colors.button_background_1);
    colors.button_selected_text = 0x31373b;

    colors.text_color = 0x2b2e31;
    colors.text_color_disabled = disabled;

    colors.text_selection_color = 0xffffff;
    colors.text_selection_background = accent;
    return colors;
}

auto ThemePlasma::get_dark_colors(int32_t accent) -> ColorStyle {
    auto colors = ColorStyle();
    colors.window_background = 0xeff0f1;

    colors.frame_normal_color1 = 0xbbbcbd;
    colors.frame_normal_color2 = 0xbbbcbd;
    colors.frame_normal_color3 = 0xbbbcbd;
    colors.frame_normal_color4 = 0xbbbcbd;

    colors.frame_hover_color1 = 0x3daee9;
    colors.frame_hover_color2 = 0x3daee9;
    colors.frame_hover_color3 = 0x3daee9;
    colors.frame_hover_color4 = 0x3daee9;

    colors.frame_selected_color1 = 0x3daee9;
    colors.frame_selected_color2 = 0x3daee9;
    colors.frame_selected_color3 = 0x3daee9;
    colors.frame_selected_color4 = 0x3daee9;

    colors.frame_disabled_color1 = 0xd1d2d3;
    colors.frame_disabled_color2 = 0xd1d2d3;
    colors.frame_disabled_color3 = 0xd1d2d3;
    colors.frame_disabled_color4 = 0xd1d2d3;

    colors.input_background_normal = 0xffffff;
    colors.input_background_hover = 0x0;
    colors.input_background_disabled = 0x0;
    colors.input_background_selected = 0x0;

    colors.button_background_1 = 0xfcfcfc;
    colors.button_background_2 = 0xf5f5f5;
    colors.button_selected_background = 0xd6ecf8;
    colors.button_selected_text = 0x31373b;
    //    colors.button_selected_border = 0x3daee9; // todo - needed?

    colors.text_color = 0x2b2e31;
    colors.text_color_disabled = 0x737577;

    colors.text_selection_color = 0xffffff;
    colors.text_selection_background = 0x3daee9;
    return colors;
}

auto ThemeVision::draw_listview_background(Bitmap &content, const bool has_focus,
                                           bool draw_background) -> void {

    draw_frame(content, {0, 0}, content.size, has_focus ? FrameStyles::Hover : FrameStyles::Normal,
               FrameSize::SingleFrame);
    if (draw_background) {
        content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2,
                          colors.input_background_normal);
    }
}

auto ThemePlasma::draw_window_background(Bitmap &content) -> void {
    content.fill_rect(0, 0, content.size.width, content.size.height, colors.window_background);
    content.line(0, 0, content.size.width - 1, 0, colors.frame_disabled_color1);
}

auto ThemePlasma::draw_scrollbar_background(Bitmap &content) -> void {
    auto the_width = content.size.width;
    auto the_height = content.size.height;
    auto topleft = Position{0, 0};

    content.fill_rect(0, 0, the_width, the_height, colors.window_background);
    draw_frame(content, topleft, {the_width, the_height - 1}, FrameStyles::Normal,
               FrameSize::SingleFrame);
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

    content.line(2, content.size.height - 1, content.size.width - 2, content.size.height - 1,
                 colors.frame_disabled_color1);

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
    draw_frame(content, {0, 0}, content.size, has_focus ? FrameStyles::Hover : FrameStyles::Normal,
               FrameSize::SingleFrame);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2,
                      has_focus ? colors.input_background_hover : colors.input_background_normal);
}

auto ThemePlasma::draw_listview_background(Bitmap &content, const bool has_focus,
                                           const bool draw_background) -> void {
    draw_frame(content, {0, 0}, content.size, has_focus ? FrameStyles::Hover : FrameStyles::Normal,
               FrameSize::SingleFrame);
    auto background = has_focus ? colors.input_background_selected : colors.input_background_normal;
    if (draw_background) {
        content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
    }
}
