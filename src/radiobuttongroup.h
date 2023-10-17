#ifndef RADIOBUTTONGROUP_H
#define RADIOBUTTONGROUP_H

#include "widget.h"

class RadioButtonGroup : public Widget
{
  public:
    RadioButtonGroup(Position pos, size_t width, std::list<std::string> items);
};

#endif // RADIOBUTTONGROUP_H
