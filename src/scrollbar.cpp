#include "scrollbar.h"
#include <button.h>
#include <spdlog/spdlog.h>

ScrollBar::ScrollBar(Position position, int length, bool horizontal) : Widget(position, {}, 0) {
    this->is_horizontal = horizontal;
    auto button_size = Size{32, 32};
    if (horizontal) {
        auto button_position = Position{0, 0};
        this->content.resize({length, 32});
        this->down_button = std::make_shared<Button>(button_position, button_size, "<");
        this->add(this->down_button);
        this->down_button->content.background_color = 0x22ffff;

        button_position.x = content.size.width - 32;
        this->up_button = std::make_shared<Button>(button_position, button_size, ">");
        this->up_button->content.background_color = 0xffff22;
        this->add(this->up_button);
    } else {
        this->content.resize({32, length});
    }

    this->up_button->on_button_click = []() { spdlog::info("value++"); };
    this->down_button->on_button_click = []() { spdlog::info("value--"); };
}
