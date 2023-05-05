#include "textfield.h"
#include <spdlog/spdlog.h>
#include <theme.h>

#include <spdlog/spdlog.h>

TextField::TextField(Position position, Size size) :
    Widget(position, size, 0)
{
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

TextField::~TextField()
{
    timer.stop();
}

auto TextField::draw() -> void
{
    content.fill_rect(0,0, content.size.width, content.size.height, 0xffffff);
    theme->draw_widget_frame(content, this->has_focus, true);
    content.write_fixed( Position{5, 5}, this->text, 0);

    if (this->cursor_on && this->has_focus) {
        auto padding = 5;
        content.draw_rectangle(
            padding, padding,
            1, content.size.height-padding*2,
            0, 0
        );
    }
}

auto TextField::on_keyboard(const EventKeyboard &event) -> void
{
    switch (event.key)
    {
    case KeyCodes::Enter:
        spdlog::info("Pressed enter");
        break;

    default:
        // TODO handle non ascii input
        if ((int)event.key > ' ' && (int)event.key < 128) {
            char ascii = (char)event.key;
            spdlog::info("new ascii char: {}", ascii);
            text += (char)event.key;
        } else {
            spdlog::info("KeyCode = {}", (int) event.key);
        }
        needs_redraw = true;
        break;
    }
}


auto TextField::on_focus_change(bool new_state) -> void
{
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

auto TextField::on_remove() -> void {
    timer.stop();
}
