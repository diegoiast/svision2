/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "scrollbar.h"
#include "theme.h"
#include <button.h>

ScrollBar::ScrollBar(Position position, int length, bool horizontal, int maximum)
    : Widget(position, {}, 0) {
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

    this->up_button->on_button_click = [this]() { step_up(); };
    this->down_button->set_auto_repeat(500);
    this->down_button->on_button_click = [this]() { step_down(); };
    this->up_button->set_auto_repeat(500);

    this->draw_background = true;
    this->frame = {FrameStyles::Normal, FrameSize::SingleFrame};
    this->is_horizontal = horizontal;
    this->maximum = maximum;
    this->update_thumb_size();
}

auto ScrollBar::draw() -> void {
    auto theme = get_theme();
    theme->draw_scrollbar_background(content);
    Widget::draw();

    if (is_horizontal) {
        auto offset = down_button->content.size.width + thumb_position;
        content.fill_rect(offset, 0, thumb_size, content.size.height,
                          theme->colors.button_background_1);
        content.draw_rectangle(offset, 0, thumb_size, content.size.height,
                               theme->colors.frame_normal_color1,
                               theme->colors.frame_normal_color2);

    } else {
        auto offset = down_button->content.size.height + thumb_position;
        content.fill_rect(0, offset, content.size.width, thumb_size,
                          theme->colors.button_background_1);
        content.draw_rectangle(0, offset, content.size.width, thumb_size,
                               theme->colors.frame_normal_color1,
                               theme->colors.frame_normal_color2);
    }
}

auto ScrollBar::set_value(int value) -> void {
    if (value == this->value) {
        return;
    }

    if (value < minimum) {
        value = minimum;
    }
    if (value >= maximum) {
        value = maximum;
    }
    this->value = value;
    update_thumb_size();
    up_button->invalidate();
    down_button->invalidate();
    invalidate();

    if (did_change) {
        this->did_change(this, value);
    }
}

auto ScrollBar::set_values(int minimum, int maximum, int value, int step) -> void {
    if (value < minimum) {
        value = minimum;
    }
    if (value >= maximum) {
        value = maximum;
    }
    if (step == 0) {
        step = (maximum - minimum) / 10;
    }
    this->minimum = minimum;
    this->maximum = maximum;
    this->step = step;
    this->value = value;
    update_thumb_size();
    up_button->invalidate();
    down_button->invalidate();
    invalidate();

    if (did_change) {
        // TODO - has any of the values been modified?
        this->did_change(this, value);
    }
}

auto ScrollBar::step_up() -> void {
    this->value += this->step;
    this->value = std::min(this->maximum, this->value);
    this->up_button->is_enabled = this->value < this->maximum;
    this->down_button->is_enabled = this->value > this->minimum;
    this->up_button->invalidate();
    this->down_button->invalidate();
    update_thumb_size();
    invalidate();
    if (did_change) {
        this->did_change(this, value);
    }
}

auto ScrollBar::step_down() -> void {
    this->value -= this->step;
    this->value = std::max(this->minimum, this->value);
    this->up_button->is_enabled = this->value < this->maximum;
    this->down_button->is_enabled = this->value > this->minimum;
    this->up_button->invalidate();
    this->down_button->invalidate();
    update_thumb_size();
    invalidate();
    if (did_change) {
        this->did_change(this, value);
    }
}

auto ScrollBar::update_thumb_size() -> void {
    auto padding = is_horizontal
                       ? up_button->content.size.width + down_button->content.size.width
                       : up_button->content.size.height + down_button->content.size.height;
    auto length = is_horizontal ? content.size.width : content.size.height;
    auto range = maximum - minimum;

    auto draw_area = is_horizontal ? content.size.width - down_button->content.size.width * 2
                                   : content.size.height - down_button->content.size.height * 2;

    if (draw_area >= range) {
        // If we have enough space thumb size, is relative to widget/value
        thumb_size = length - padding - range;
        if (thumb_size < padding) {
            thumb_size = padding;
        }
        thumb_position = value - minimum;
    } else {
        // auto generate by chatgpt FTW :)

        auto available_size = is_horizontal
                                  ? content.size.width - down_button->content.size.width * 2
                                  : content.size.height - down_button->content.size.height * 2;
        auto minimum_thumb_size =
            is_horizontal ? down_button->content.size.width : down_button->content.size.height;
        auto maximum_thumb_size = available_size;
        auto visible_range = range / step;
        auto thumb_range = 0;

        thumb_size = range == 0
                         ? maximum_thumb_size
                         : std::max(minimum_thumb_size, available_size * (visible_range / range));
        thumb_range = available_size - thumb_size;
        thumb_position = range == 0 ? 0 : (value - minimum) * thumb_range / range;
    }
    // technically - `invalidate()` should be called, but it is called usually
    // right after this function
}
