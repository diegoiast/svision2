#include "scrollbar.h"
#include <button.h>
#include <spdlog/spdlog.h>

ScrollBar::ScrollBar(Position position, int length, bool horizontal, int maximum) : Widget(position, {}, 0) {
    this->is_horizontal = horizontal;
    this->maximum = maximum;
    auto button_size = Size{32, 32};
    if (horizontal) {
        auto button_position = Position{0, 0};
        this->content.resize({length, 32});
        this->down_button = std::make_shared<Button>(button_position, button_size, "<");
        this->add(this->down_button);

        button_position.x = content.size.width - 32;
        this->up_button = std::make_shared<Button>(button_position, button_size, ">");
        this->add(this->up_button);
    } else {
        auto button_position = Position{0, 0};
        this->content.resize({32, length});
        this->down_button = std::make_shared<Button>(button_position, button_size, "^");
        this->add(this->down_button);

        button_position.y = content.size.height - 32;
        this->up_button = std::make_shared<Button>(button_position, button_size, "*");
        this->add(this->up_button);
    }

    this->up_button->on_button_click = [this]() {step_up(); };
    this->down_button->on_button_click = [this]() { step_down(); };
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


