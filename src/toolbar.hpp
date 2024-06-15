#pragma once

#include <functional>
#include <map>
#include <widget.h>

struct Button;

struct KeyboardShortCut {
    KeyCodes keyboardShortcut;
    int modifiers;
};

struct Shortcut {
    int id = 0;
    std::function<void(Shortcut &, int)> on_trigger;
    std::string title;
    std::string hint;
    std::string icon_name;

    Shortcut(int id, std::string_view title, std::string_view icon,
             std::function<void(Shortcut &, int)> callback) {
        this->id = id;
        this->title = title;
        this->icon_name = icon;
        this->on_trigger = callback;
    }
};

struct Toolbar : Widget {
    Toolbar();
    virtual auto size_hint() const -> Size override;
    auto add_shortcut(int id, std::string_view title, std::string_view icon,
                      std::function<void(Shortcut &, int)> callback) -> std::shared_ptr<Toolbar>;
    auto add_shortcut(std::shared_ptr<Shortcut> s) -> std::shared_ptr<Toolbar>;
    auto update_toolbar() -> void;

    std::map<std::shared_ptr<Button>, std::shared_ptr<Shortcut>> shortcuts;
};
