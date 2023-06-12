#include "scrollbar.h"
#include "theme.h"
#include <button.h>
#include <spdlog/spdlog.h>

ScrollBar::ScrollBar(Position position, int length, bool horizontal, int maximum) : Widget(position, {}, 0) {
    this->is_horizontal = horizontal;
    this->maximum = maximum;

    // TODO button size is found in the theme. Theme is not set up until the
    // widget is added to window. Which means - all this code should be called
    // inside a callback which gets called when added to a window. No such
    // event exist yet.
    // TODO button text should be images (?). However - buttons do not support
    // images yet.
    auto buttons_size = 24;
    auto button_size = Size{buttons_size, buttons_size};
    if (horizontal) {
        auto button_position = Position{0, 0};
        this->content.resize({length, buttons_size});
        this->down_button = std::make_shared<Button>(button_position, button_size, "<");
        this->add(this->down_button);

        button_position.x = content.size.width - buttons_size;
        this->up_button = std::make_shared<Button>(button_position, button_size, ">");
        this->add(this->up_button);
    } else {
        auto button_position = Position{0, 0};
        this->content.resize({buttons_size, length});
        this->down_button = std::make_shared<Button>(button_position, button_size, "^");
        this->add(this->down_button);

        button_position.y = content.size.height - buttons_size;
        this->up_button = std::make_shared<Button>(button_position, button_size, "*");
        this->add(this->up_button);
    }

    this->up_button->on_button_click = [this]() {step_up(); };
    this->down_button->on_button_click = [this]() { step_down(); };
}

auto ScrollBar::draw() -> void {
    auto theme = get_theme();
    theme->draw_scrollbar_background(content);
    Widget::draw();
}

auto ScrollBar::set_value(int minimum, int maximum, int step) -> void
{
    if (step == 0) {
        step = (maximum - minimum) / 10;
    }
    this->minimun = minimum;
    this->maximum = maximum;
    this->step = step;
    invalidate();
}

auto ScrollBar::step_up() -> void
{
    this->value += this->step;
    this->value = std::min(this->maximum, this->value);
    spdlog::info("value++ -> value = {} (step={})", value, step);
    invalidate();
}

auto ScrollBar::step_down() -> void
{
    this->value -= this->step;
    this->value = std::max(this->minimun, this->value);
    spdlog::info("value-- -> value = {} (step={})", value, step);
    invalidate();
}
