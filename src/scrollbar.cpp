/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "scrollbar.h"
#include "theme.h"
#include <button.h>

auto make_buttons(ScrollBar &sb, int length, bool horizontal) {
    // TODO button text should be images (?). However - buttons do not support
    // images yet.
    auto default_buttons_size =
        horizontal ? sb.get_padding().get_vertical() : sb.get_padding().get_horizontal();
    auto button_size = Size{default_buttons_size, default_buttons_size};

    if (horizontal) {
        auto button_position = Position{0, 0};
        sb.content.resize({length, default_buttons_size});

        if (!sb.down_button) {
            sb.down_button = std::make_shared<Button>(button_position, button_size, "<");
            sb.add(sb.down_button);
        } else {
            sb.down_button->position = button_position;
            sb.down_button->content.resize(button_size);
        }

        button_position.x = sb.content.size.width - default_buttons_size;
        if (!sb.up_button) {
            sb.up_button = std::make_shared<Button>(button_position, button_size, ">");
            sb.add(sb.up_button);
        } else {
            sb.up_button->position = button_position;
            sb.up_button->content.resize(button_size);
        }
    } else {
        auto button_position = Position{0, 0};
        sb.content.resize({default_buttons_size, length});

        if (!sb.down_button) {
            sb.down_button = std::make_shared<Button>(button_position, button_size, "^");
            sb.add(sb.down_button);
        } else {
            sb.down_button->position = button_position;
            sb.down_button->content.resize(button_size);
        }

        button_position.y = sb.content.size.height - default_buttons_size;
        if (!sb.up_button) {
            sb.up_button = std::make_shared<Button>(button_position, button_size, "*");
            sb.add(sb.up_button);
        } else {
            sb.up_button->position = button_position;
            sb.up_button->content.resize(button_size);
        }
    }

    sb.up_button->invalidate();
    sb.down_button->invalidate();
}

ScrollBar::ScrollBar(Position position, int length, bool horizontal, int maximum)
    : Widget(position, {}, 0) {

    make_buttons(*this, length, horizontal);

    this->up_button->on_button_click = [this]() { step_up(); };
    this->down_button->set_auto_repeat(500);
    this->down_button->on_button_click = [this]() { step_down(); };
    this->up_button->set_auto_repeat(500);

    this->draw_background = true;
    this->frame = {FrameStyles::Normal, FrameSize::SingleFrame};
    this->is_horizontal = horizontal;
    this->maximum = maximum;
    this->update_thumb_size();
    this->padding_style = PaddingStyle::ScrollBar;
}

auto ScrollBar::draw() -> void {
    auto my_theme = get_theme();
    content.fill(0xffff00);
    my_theme->draw_scrollbar_background(content);
    Widget::draw();

    // TODO - move this code to the theme
    if (is_horizontal) {
        auto offset = down_button->content.size.width + thumb_position;
        content.fill_rect(offset, 0, thumb_size, content.size.height,
                          my_theme->colors.button_background_1);
        content.draw_rectangle(offset, 0, thumb_size, content.size.height,
                               my_theme->colors.frame_normal_color1,
                               my_theme->colors.frame_normal_color2);

    } else {
        auto offset = down_button->content.size.height + thumb_position;
        content.fill_rect(0, offset, content.size.width, thumb_size,
                          my_theme->colors.button_background_1);
        content.draw_rectangle(0, offset, content.size.width, thumb_size,
                               my_theme->colors.frame_normal_color1,
                               my_theme->colors.frame_normal_color2);
    }
}

auto ScrollBar::on_resize() -> void {
    make_buttons(*this, is_horizontal ? content.size.width : content.size.height, is_horizontal);
}

auto ScrollBar::size_hint() const -> Size {
    if (is_horizontal)
        return {0, get_padding().get_vertical()};
    else
        return {get_padding().get_horizontal(), 0};
}

auto ScrollBar::set_value(int new_value) -> void {
    if (new_value == this->value) {
        return;
    }

    if (new_value < minimum) {
        new_value = minimum;
    }
    if (new_value >= maximum) {
        new_value = maximum;
    }
    this->value = new_value;
    update_thumb_size();
    up_button->invalidate();
    down_button->invalidate();
    invalidate();

    if (did_change) {
        this->did_change(this, new_value);
    }
}

auto ScrollBar::set_values(int min, int max, int new_value, int new_step) -> void {
    if (new_value < min) {
        new_value = min;
    }
    if (new_value >= max) {
        new_value = max;
    }
    if (new_step == 0) {
        new_step = (max - min) / 10;
    }
    this->minimum = min;
    this->maximum = max;
    this->step = new_step;
    this->value = new_value;
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
    auto thumb_padding = is_horizontal
                             ? up_button->content.size.width + down_button->content.size.width
                             : up_button->content.size.height + down_button->content.size.height;
    auto length = is_horizontal ? content.size.width : content.size.height;
    auto range = maximum - minimum;

    auto draw_area = is_horizontal ? content.size.width - down_button->content.size.width * 2
                                   : content.size.height - down_button->content.size.height * 2;

    if (draw_area >= range) {
        // If we have enough space thumb size, is relative to widget/value
        thumb_size = length - thumb_padding - range;
        if (thumb_size < thumb_padding) {
            thumb_size = thumb_padding;
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
