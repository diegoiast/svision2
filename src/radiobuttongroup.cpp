#include "radiobuttongroup.h"
#include "checkbox.h"
#include <spdlog/spdlog.h>

auto on_radio_button_selected(RadioButtonGroup &group, Checkbox &checkbox) -> void {
    for (auto &cb : group.radio_buttons) {
        if (cb.get() == &checkbox) {
            cb->set_checked(EventPropagation::handled);
        } else {
            cb->set_unchecked(EventPropagation::handled);
        }
    }

    //    group.on_radio_button_selected();
}

RadioButtonGroup::RadioButtonGroup(Position position, size_t width,
                                   const std::list<std::string> &items)
    : Widget(position, {}, 0) {
    auto padding = 5;
    auto p = Position{0, 0};
    auto height = 0;

    for (auto &item : items) {
        auto cb = add(std::make_shared<Checkbox>(p, width, item));
        cb->shape = CheckboxShape::RadioButton;
        cb->on_checkbox_change = [this](Checkbox &cb) { on_radio_button_selected(*this, cb); };
        radio_buttons.push_back(cb);
        p.y += cb->content.size.height + padding;
        height += cb->content.size.height + padding;
    }
    content.resize(width, height);
}
