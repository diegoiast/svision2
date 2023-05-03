#pragma once

#include <widget.h>
#include <timer.h>

struct TextField : Widget {
    TextField(Position position, Size size);
    virtual ~TextField() override;
    virtual auto draw() -> void override;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_remove() -> void override;

  private:
    bool cursor_on = false;
    Timer timer;
};
