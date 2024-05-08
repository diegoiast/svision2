#pragma once

#include <functional>
#include <map>
#include <widget.h>

struct Button;
template <typename T> using P = std::shared_ptr<T>;

struct KeyboardShortCut {
    KeyCodes keyboardShortcut;
    int modifiers;
};

struct Shortcut {
    int id = 0;
    std::string_view title;
    std::function<void(Shortcut &, int)> on_trigger;
    std::string_view hint;

    Shortcut(int id, std::string_view title, std::function<void(Shortcut &, int)> callback) {
        this->id = id;
        this->title = title;
        this->on_trigger = callback;
    }
};

struct Toolbar : Widget {
    Toolbar();
    virtual auto size_hint() const -> Size override;
    auto add_shortcut(P<Shortcut> s) -> P<Toolbar>;
    auto update_toolbar() -> void;

    std::map<P<Button>, P<Shortcut>> shortcuts;
};
