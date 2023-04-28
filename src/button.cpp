#include <button.h>
#include <spdlog/spdlog.h>

Button::Button(Position pp, Size size, std::string text) : Widget(pp, size, 0) {
    this->text = text;
}

Button::Button(Position pp, Size size, std::string text, std::function<void()> on_button_click)
    : Widget(pp, size, 0) {
    this->text = text;
    this->on_button_click = on_button_click;
}

auto Button::draw() -> void {
    auto background_color = MakeColor(0x80, 0x80, 0x80);
    auto line1 = MakeColor(0xe0, 0xe0, 0xe0);
    auto line2 = MakeColor(0x80, 0x80, 0x80);
    auto shadow_offset = 0;
    auto text_offset = 0;

    switch (state) {
    case States::Normal:
        line1 = MakeColor(0xe0, 0xe0, 0xe0);
        line2 = MakeColor(0x80, 0x80, 0x80);
        break;
    case States::Hovered:
        // line1 = MakeColor(0x00, 0xff, 0x00);
        // line2 = MakeColor(0x00, 0xff, 0x00);
        background_color = MakeColor(0x90, 0x90, 0x90);
        break;
    case States::ClickedInside:
        line2 = MakeColor(0xe0, 0xe0, 0xe0);
        line1 = MakeColor(0x80, 0x80, 0x80);
        background_color = MakeColor(0x90, 0x90, 0x90);
        shadow_offset = 1;
        break;
    case States::ClickedOutside:
        line1 = MakeColor(0xe0, 0xe0, 0xe0);
        line2 = MakeColor(0x80, 0x80, 0x80);
        background_color = MakeColor(0x90, 0x90, 0x90);
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
    content.write_fixed(Position{7 + shadow_offset, 7 + shadow_offset}, text, 0);
    content.write_fixed(Position{5 + text_offset, 5 + text_offset}, text, 0xffffff);
}

auto Button::on_hover(const EventMouse &event) -> void {
    // default implementation demands redraw, we
    // don't need this by default
}

auto Button::on_mouse_enter() -> void {
    mouse_over = true;
    switch (state) {
    case States::ClickedInside:
        state = States::ClickedOutside;
        needs_redraw = true;
        break;
    case States::ClickedOutside:
        state = States::ClickedInside;
        needs_redraw = true;
        break;
    case States::Hovered:
        break;
    case States::Normal:
        state = States::Hovered;
        needs_redraw = true;
        break;
    }
}

auto Button::on_mouse_leave() -> void {
    mouse_over = false;

    switch (state) {
    case States::ClickedInside:
        state = States::ClickedOutside;
        needs_redraw = true;
        break;
    case States::ClickedOutside:
        state = States::ClickedInside;
        needs_redraw = true;
        break;
    case States::Hovered:
        state = States::Normal;
        needs_redraw = true;
        break;
    case States::Normal:
        break;
    }
}

auto Button::on_mouse_click(const EventMouse &event) -> void {
    switch (state) {
    case States::ClickedInside:
        state = States::Hovered;
        needs_redraw = true;
        if (on_button_click) {
            on_button_click();
        }
        break;
    case States::ClickedOutside:
        if (event.pressed) {
            state = States::ClickedInside;
            needs_redraw = true;
        } else {
            state = States::Normal;
            needs_redraw = true;
            spdlog::debug("Button click aborted");
        }
        break;
    case States::Hovered:
        if (event.pressed) {
            state = States::ClickedInside;
            needs_redraw = true;
        }
        break;
    case States::Normal:
        if (event.pressed) {
            state = States::ClickedInside;
            needs_redraw = true;
        }
        break;
    }
}
