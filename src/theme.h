#pragma once

#include <bitmap.h>

enum class ButtonStates {
    Normal,
    Hovered,
    ClickedInside,
    ClickedOutside,
};

struct Theme {
    virtual auto init() -> void = 0;
    virtual auto draw_window_background(Bitmap &content) -> void = 0;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, ButtonStates state,
                             const std::string &text) -> void = 0;
    virtual auto needs_frame_for_focus() -> bool = 0;
};

struct ThemeOldie : Theme {
    static const int32_t background_color = 0xeff0f1;
    static const int32_t background_color_normal = MakeColor(0xb0, 0xb0, 0xb0);
    static const int32_t background_color_hover = MakeColor(0xc3, 0xc3, 0xc3);
    static const int32_t line_color1 = MakeColor(0xff, 0xff, 0xff);
    static const int32_t line_color2 = MakeColor(0x64, 0x64, 0x64);

    virtual auto init() -> void override{};
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, ButtonStates state,
                             const std::string &text) -> void override;
    virtual auto needs_frame_for_focus() -> bool override { return true; };
};

struct ThemeRedmond : Theme {
    const int32_t background_color = 0xc0c0c0;
    const int32_t background_color_hover = 0xc8c8c8;
    static const int32_t line_color1 = 0xFFFFFF;
    static const int32_t line_color2 = 0x000000;

    static const int32_t line_color3 = 0xFFFFFF;
    static const int32_t line_color4 = 0x808080;

    static const int32_t text_color = 0x000000;

    virtual auto init() -> void override{};
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, ButtonStates state,
                             const std::string &text) -> void override;
    virtual auto needs_frame_for_focus() -> bool override { return true; };
};

struct ThemePlasma : Theme {
    static const int32_t text_color = 0x2b2e31;
    static const int32_t text_color_disabled = 0x737577;

    static const int32_t window_background_color = 0xeff0f1;
    static const int32_t button_background_1 = 0xfcfcfc;
    static const int32_t button_background_2 = 0xf5f5f5;
    static const int32_t button_border = 0xbbbcbd;
    static const int32_t button_shadow = 0xd1d2d3;

    const int32_t button_selected_background = 0xd6ecf8;
    const int32_t button_selected_border = 0x3daee9;
    const int32_t button_selected_text = 0x31373b;

    virtual auto init() -> void override{};
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, ButtonStates state,
                             const std::string &text) -> void override;
    virtual auto needs_frame_for_focus() -> bool override { return false; };
};
