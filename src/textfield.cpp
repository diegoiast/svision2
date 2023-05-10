#include "textfield.h"
#include <spdlog/spdlog.h>
#include <theme.h>

#include <spdlog/spdlog.h>

TextField::TextField(Position position, Size size) : Widget(position, size, 0) {
    timer.millies = 750;
    timer.repeating = true;
    timer.callback = [this]() {
        this->cursor_on = !this->cursor_on;
        this->needs_redraw = true;
        if (this->window) {
            this->window->invalidate();
        }
    };
    timer.initialize();
    this->can_focus = true;
}

TextField::~TextField() { timer.stop(); }

auto TextField::draw() -> void {
    auto padding = 5;
    auto display_text_logical = (content.size.width - padding * 2) / 8;
    auto display_text = this->text.substr(display_from, display_from + display_text_logical);
    auto center_y = (content.size.height - 16) / 2;
    content.fill_rect(0, 0, content.size.width, content.size.height, 0xffffff);
    theme->draw_widget_frame(content, this->has_focus, true);
    content.write_fixed(Position{padding, center_y}, display_text, 0);

    if (this->cursor_on && this->has_focus) {
        auto position_x = padding + (cursor_position - display_from) * 8;
        content.draw_rectangle(
            position_x, padding,
            1, content.size.height - padding * 2, 0, 0);
    }
}

auto TextField::on_keyboard(const EventKeyboard &event) -> void {
    switch (event.key) {
    case KeyCodes::Enter:
        spdlog::info("Pressed enter");
        break;
    case KeyCodes::ArrowLeft:
        if (cursor_position > 0) {
            cursor_position --;
            needs_redraw = true;
            cursor_on = true;
            ensure_cursor_visible();
        }
        break;
    case KeyCodes::ArrowRight:
        if (cursor_position < text.length()) {
            cursor_position ++;
            needs_redraw = true;
            cursor_on = true;
            ensure_cursor_visible();
        }
        break;
    case KeyCodes::Home:
        if (cursor_position != 0) {
            display_from = 0;
            cursor_position = 0;
            needs_redraw = true;
            cursor_on = true;

        }
        break;
    case KeyCodes::End:
        cursor_position = text.length();
        needs_redraw = true;
        cursor_on = true;
        ensure_cursor_visible();
        break;
    case KeyCodes::Delete:
        text.erase(cursor_position,1);
        needs_redraw = true;
        cursor_on = true;
        ensure_cursor_visible();
        break;
    case KeyCodes::Backspace:
        if (cursor_position > 0) {
            text.erase(cursor_position-1,1);
            cursor_position --;
            needs_redraw = true;
            cursor_on = true;
            ensure_cursor_visible();
        }
        break;
    default:
        // TODO handle non ascii input
        auto is_control_pressed = event.is_control_pressed();
        auto is_a_pressed = event.key == (KeyCodes)'A' || event.key == (KeyCodes)'a';
        if (is_a_pressed && is_control_pressed) {
            spdlog::info("Selecting all");
            break;
        }
        if ((int)event.key >= ' ' && (int)event.key <= 128) {
            char ascii = (char)event.key;
            spdlog::info("new ascii char: {}", ascii);

            if (cursor_position > text.length()) {
                text += (char)event.key;
            } {
                text.insert(cursor_position, 1, ascii);
            }
            cursor_position += 1;
            needs_redraw = true;
            cursor_on = true;
            ensure_cursor_visible();
        } else {
            spdlog::info("KeyCode = {:x}", (int)event.key);
        }
        break;
    }
}

auto TextField::on_mouse_click(const EventMouse &event) -> void {
    if (!event.pressed || event.button != 1 || !event.is_local) {
        return;
    }

    auto padding = 5;
    auto pos = (event.x - padding) / 8;
    cursor_position = display_from + pos;
    cursor_position = std::min((size_t)cursor_position, text.length());
    needs_redraw = true;
    cursor_on = true;
}

auto TextField::on_focus_change(bool new_state) -> void {
    if (new_state) {
        timer.start();
        this->cursor_on = true;
    } else {
        timer.stop();
    }

    this->needs_redraw = true;
    if (this->window) {
        this->window->invalidate();
    }
}

auto TextField::on_remove() -> void { timer.stop(); }

auto TextField::ensure_cursor_visible() -> void
{
    auto padding = 5;
    auto max_x_position = content.size.width - padding*2;
    auto cursor_visual_position = (cursor_position - display_from) * 8 + padding;
//    auto visible_text = text.substr(display_from);
//    spdlog::info("Cursor position = {}/{}", cursor_visual_position, max_x_position);

    while (cursor_visual_position > max_x_position) {
        display_from ++;
        cursor_visual_position = (cursor_position - display_from) * 8 + padding;
//        visible_text = text.substr(display_from);
        spdlog::info("Cursor position = {}/{}", cursor_visual_position, max_x_position);
    }

    while (display_from > cursor_position) {
        display_from =
            cursor_position-1;
    }
    if (display_from < 0) {
        display_from = 0;
    }
}
