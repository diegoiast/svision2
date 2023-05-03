#include "textfield.h"
#include <spdlog/spdlog.h>

#include <spdlog/spdlog.h>

TextField::TextField(Position position, Size size) :
    Widget(position, size, 0)
{
    timer.millies = 1000;
    timer.repeating = true;
    timer.callback = [this]() {
        this->cursor_on = !this->cursor_on;
        this->needs_redraw = true;
//      TODO:
//        this->window->invalidate();
        spdlog::info("Cursor is {}", this->cursor_on);
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
    if (cursor_on) {
        content.fill(0x80c080);
    } else {
        content.fill(0xc0c0a0);
    }
}

auto TextField::on_focus_change(bool new_state) -> void
{
    if (new_state) {
        spdlog::info("Starting cursor timer");
        timer.start();
    } else {
        spdlog::info("Stopping cursor timer");
        timer.stop();
    }
}

auto TextField::on_remove() -> void {
    timer.stop();
}
