#pragma once

#include <timer.h>
#include <widget.h>

struct SelectionRange {
    int start;
    int end;
};

struct TextField : Widget {
    TextField(Position position, Size size);
    virtual ~TextField() override;
    virtual auto draw() -> void override;
    virtual auto on_keyboard(const EventKeyboard &) -> void override;
    virtual auto on_mouse_click(const EventMouse &event) -> void override;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_remove() -> void override;

    auto select_all() -> void;
    auto select_none() -> void;
    auto get_selected_text() -> const std::string;
    auto has_selection() -> bool { return selection.end - selection.start > 0; }
    auto get_text() const -> std::string { return text; }
    auto set_text(const std::string_view new_text);

    int padding_start = 5;
    int padding_end = 5;
  private:
    auto ensure_cursor_visible() -> void;

    std::string text = "";
    SelectionRange selection = {0, 0};

    Timer timer;
    bool cursor_on = false;
    int cursor_position = 0;
    int display_from = 0;
};
