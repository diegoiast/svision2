#pragma once

#include <timer.h>
#include <widget.h>

struct TextField : Widget {
    TextField(Position position, Size size);
    virtual ~TextField() override;
    virtual auto draw() -> void override;
    virtual auto on_keyboard(const EventKeyboard &) -> void;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_remove() -> void override;

  private:
    bool cursor_on = false;
    std::string text = "";
    Timer timer;
};
