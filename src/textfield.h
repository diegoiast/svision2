#pragma once

#include <timer.h>
#include <widget.h>

struct TextField : Widget {
    TextField(Position position, Size size);
    virtual ~TextField() override;
    virtual auto draw() -> void override;
    virtual auto on_keyboard(const EventKeyboard &) -> void override;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_remove() -> void override;

  private:
    auto ensure_cursor_visible() -> void;

    bool cursor_on = false;
    std::string text = "";
    Timer timer;
    int cursor_position = 0;
    int display_from = 0;
};
