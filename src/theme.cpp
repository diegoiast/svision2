#include "theme.h"

auto ThemeOldie::draw_window_background(Bitmap &content) -> void {
    content.fill(background_color);
}

auto ThemeOldie::draw_button(Bitmap &content, ButtonStates state, const std::string &text) -> void {
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


auto ThemeRedmond::draw_window_background(Bitmap &content) -> void
{
    content.fill(background_color);
}

auto ThemeRedmond::draw_button(Bitmap &content, ButtonStates state, const std::string &text) -> void
{
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
        shadow_offset = 1;
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

    content.fill_rect(0, 0, content.size.width , content.size.height, 0xffaabb);
    content.draw_rectangle(0, 0, content.size.width, content.size.height , line1, line2);
    content.draw_rectangle(1, 1, content.size.width-2, content.size.height - 2, line3, line4);

    content.fill_rect(2, 2, content.size.width - 4, content.size.height - 4, background_color);
    content.write_fixed(Position{shadow_padding + shadow_offset, shadow_padding + shadow_offset},
                        text, 0);
    content.write_fixed(Position{text_padding + text_offset, text_padding + text_offset}, text,
                        0xffffff);

}


auto ThemePlasma::draw_window_background(Bitmap &content) -> void {
    content.fill_rect(0, 0, content.size.width, content.size.height, ThemePlasma::window_background_color);
    content.line(0, 0, content.size.width - 1, 0, button_border);
}

auto ThemePlasma::draw_button(Bitmap &content, ButtonStates state, const std::string &text) -> void {
    auto text_padding = 5;
    auto shadow_padding = text_padding + 1;

    auto background1 = ThemePlasma::button_background_1;
    auto background2 = ThemePlasma::button_background_2;
    auto border = ThemePlasma::button_border;
    auto color = ThemePlasma::text_color;

    switch (state) {
    case ButtonStates::Normal:
        background1 = ThemePlasma::button_background_1;
        background2 = ThemePlasma::button_background_2;
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

    content.draw_rounded_rectangle(0, 0, content.size.width, content.size.height - 1, 5, border, border);
    content.line(2, content.size.height - 1, content.size.width - 2, content.size.height - 1,
                 button_shadow);

    // TODO - widet should be filled with real content from parent
    content.put_pixel(0, content.size.height - 1, ThemePlasma::window_background_color);
    content.put_pixel(1, content.size.height - 1, ThemePlasma::window_background_color);
    content.put_pixel(content.size.width - 1, content.size.height - 1, ThemePlasma::window_background_color);

    if (background1 == background2) {
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 3, background1);
    } else {
    content.fill_rect_gradient(1, 1, content.size.width - 2, content.size.height - 3, background1, background2);

    }
    content.write_fixed(Position{shadow_padding, shadow_padding}, text, color);
}
