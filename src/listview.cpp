#include "listview.h"
#include "scrollbar.h"
#include "spdlog/spdlog.h"
#include "theme.h"

auto ListItemAdapter::get_widget(size_t) -> PWidget {
    auto position = Position{0, 0};
    auto size = Size{20, 20};
    auto p = std::make_shared<ListItemWidget>(position, size, "");
    p->can_focus = true;
    //    p->draw_background = false;
    return p;
}

auto ListItemAdapter::set_content(PWidget widget, size_t position, ItemStatus status) -> void {
    auto item = std::dynamic_pointer_cast<ListItemWidget>(widget);
    item->text = strings.at(position);
    item->status = status;
}

ListView::ListView(Position position, Size size) : Widget(position, size, 0) {

    position.x = size.width - 24;
    position.y = 0;
    this->scrollbar = add_new<ScrollBar>(position, size.height, false);
    this->scrollbar->did_change = [this](auto *s, int value) { this->invalidate(); };
    this->draw_background = false;
    this->can_focus = true;
}

auto ListView::draw() -> void {
    get_theme()->draw_listview_background(content, has_focus, true);
    if (reserved_widgets.empty()) {
        did_adapter_update();
    }

    auto first_widget = adapter->get_widget(0);
    auto padding = 2;
    auto item_height = (first_widget->content.size.height);
    auto widget_count = this->content.size.height / item_height + 1;
    auto first_item = scrollbar->value / item_height;
    auto offset = -(scrollbar->value % item_height);

    for (auto i = 0; i < widget_count; i++) {
        auto position = Position{padding, padding + offset};
        auto size =
            Size{this->content.size.width - this->scrollbar->content.size.width - padding * 2 + 2,
                 first_widget->content.size.height};
        auto status = ItemStatus{false, false};
        auto w = reserved_widgets[i];
        if (!w) {
            continue;
        }

        if (first_item >= adapter->get_count()) {
            w->hide();
            continue;
        }
        status.is_active = this->current_item == first_item;
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
    get_theme()->draw_listview_background(content, has_focus, false);

    content.draw(scrollbar->position, scrollbar->content);
}

EventPropagation ListView::on_mouse_click(const EventMouse &event) {
    if (!event.pressed) {
        return Widget::on_mouse_click(event);
    }

    auto p = Widget::on_mouse_click(event);
    if (p == EventPropagation::handled) {
        return p;
    }

    auto first_widget = adapter->get_widget(0);
    auto padding = 2;
    auto item_height = (first_widget->content.size.height);
    auto widget_count = this->content.size.height / item_height + 1;
    auto first_item = scrollbar->value / item_height;
    //    auto offset = -(scrollbar->value % item_height);

    auto clicked_item_offset = (event.y - padding) / item_height;
    this->current_item = clicked_item_offset + first_item;
    spdlog::info("New current item = {}", this->current_item);
    invalidate();

    return EventPropagation::handled;
}

auto ListView::did_adapter_update() -> void {
    reserved_widgets.clear();
    auto first_widget = adapter->get_widget(0);
    auto item_height = first_widget->content.size.height;
    auto widget_count = this->content.size.height / item_height + 1;

    auto k = adapter->get_count() - widget_count - 1;
    if (k < 0)
        k = 0;
    if (adapter->get_count() < widget_count) {
        k = 0;
    }

    // +2 - means the frame size
    // the speed is just to make wierd funkey updates
    this->scrollbar->set_values(0, k * item_height + 2 * 2, 0, (item_height * 3) / 11);

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

void ListItemWidget::draw() {
    auto text_color = status.is_active ? 0 : 0;
    auto background_color = status.is_active ? 0x60b060 : 0xffffff;
    this->content.fill(background_color);
    this->content.write_fixed(Position{5, 5}, text, text_color);
}
