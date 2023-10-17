#include "radiobuttongroup.h"
#include "checkbox.h"

RadioButtonGroup::RadioButtonGroup(Position position, size_t width, std::list<std::string> items)
    :Widget(position, {}, 0 )
{
    auto padding = 5;
    auto p = position;
    auto height = 0;

    for( auto &item: items) {
        auto cb = add(std::make_shared<Checkbox>(p, width, item ));
        p.y += cb->content.size.height + padding;
        height += cb->content.size.height + padding;
    }

    content.resize(width, height);
}
