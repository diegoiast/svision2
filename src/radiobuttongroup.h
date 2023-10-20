#ifndef RADIOBUTTONGROUP_H
#define RADIOBUTTONGROUP_H

#include "widget.h"
#include <functional>

class Checkbox;

class RadioButtonGroup : public Widget {
  public:
    RadioButtonGroup(Position pos, size_t width, const std::list<std::string> &items);

    std::function<void(int index, Checkbox& button)> on_selected  = {};
    std::list<std::shared_ptr<Checkbox>> radio_buttons = {};
};

#endif // RADIOBUTTONGROUP_H
