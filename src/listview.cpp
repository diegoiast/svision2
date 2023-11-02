#include "listview.h"
#include "label.h"
#include "scrollbar.h"

auto ListItemAdapter::get_widget(size_t) -> PWidget {
    auto position = Position{0, 0};
    auto size = Size{20, 20};
    return std::make_shared<Label>(position, size, "");
}

auto ListItemAdapter::set_content(PWidget widget, size_t position, ItemStatus status) -> void {
    auto label = std::dynamic_pointer_cast<Label>(widget);
    label->text = strings.at(position);
}

ListView::ListView(Position position, Size size) : Widget(position, size, 0) {

    position.x = size.width - 22;
    position.y = 0;
    this->scrollbar = add_new<ScrollBar>(position, size.height, false);
    this->scrollbar->did_change = [this](auto *s, int value) { this->invalidate(); };
    this->draw_background = false;
}

auto ListView::draw() -> void {
    get_theme()->draw_widget_background(content);
    if (reserved_widgets.empty()) {
        did_adapter_update();
    }

    auto first_widget = adapter->get_widget(0);
    auto padding = 5;
    auto item_height = (first_widget->content.size.height + padding);
    auto widget_count = this->content.size.height / item_height;
    auto first_item = scrollbar->value / item_height;
    auto offset = scrollbar->value % item_height;

    for (auto i = 0; i < widget_count; i++) {
        auto position = Position{0, offset};
        auto size = Size{this->content.size.width - scrollbar->content.size.width - padding,
                         first_widget->content.size.height};
        auto status = ItemStatus{false, false};

        if (first_item >= widget_count || first_item >= adapter->get_count()) {
            continue;
        }
        auto w = reserved_widgets[i];
        w->position = position;
        w->content.resize(size);
        adapter->set_content(w, first_item, status);
        if (!w->is_visible()) {
            w->show();
        } else {
            w->invalidate();
        }
        offset += item_height;
        first_item++;
    }
    Widget::draw();
}

auto ListView::did_adapter_update() -> void {
    reserved_widgets.clear();
    auto first_widget = adapter->get_widget(0);
    auto padding = 5;
    auto item_height = (first_widget->content.size.height + padding);
    auto widget_count = this->content.size.height / item_height;

    this->scrollbar->set_values(0, adapter->get_count() * item_height, item_height);

    widget_count++;
    while (widget_count != 0) {
        // while this would work, this is unacceptable code. It does 3
        // different actions in a single line, and will become undebuggable
        // reserved_widgets.push_back(add(adapter->get_widget(widget_count)));

        auto b = adapter->get_widget(widget_count);
        b->hide();
        this->add(b);
        this->reserved_widgets.push_back(b);
        widget_count--;
    }
    this->invalidate();
}
