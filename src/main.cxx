/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

// https://zserge.com/posts/fenster/

#if defined(__linux__)
#include "platformx11.h"
#elif defined(_win32) || defined(WIN32)
#include "platformwin32.h"
#endif

#include <button.h>
#include <checkbox.h>
#include <label.h>
#include <listview.h>
#include <radiobuttongroup.h>
#include <scrollbar.h>
#include <spinbox.h>
#include <textfield.h>

#include <spdlog/spdlog.h>
#include <timer.h>

auto handle_event(std::shared_ptr<PlatformWindow> w, const Event &ev);
auto handle_event(int window_id, int widget_id, const Event &ev);

struct DebugWidget : public Widget {
    bool state_pressed = false;
    Position pos = {0, 0};
    bool unclick_inside = false;

    DebugWidget(Position position, Size size, uint32_t color) : Widget(position, size, color) {}

    auto on_hover(const EventMouse &event) -> void {
        // spdlog::info("Widget: Mouse over: {}x{}", event.x, event.y);
        pos.x = event.x;
        pos.y = event.y;
        invalidate();
    }

    auto on_mouse_enter() -> void {
        mouse_over = true;
        spdlog::info("Widget: mouse entered!");
        invalidate();
    }

    auto on_mouse_leave() -> void {
        mouse_over = false;
        spdlog::info("Widget: mouse leave!");
        invalidate();
    }

    auto on_mouse_click(const EventMouse &event) -> EventPropagation {
        if (event.type == MouseEvents::Press) {
            spdlog::info("Click inside widget, {}", fmt::ptr(this));
            state_pressed = true;
        } else if (event.type == MouseEvents::Release) {
            if (event.is_local)
                spdlog::info("Click release inside widget, {}", fmt::ptr(this));
            else if (state_pressed)
                spdlog::info("Click release outside widget, {}", fmt::ptr(this));
            state_pressed = false;
            unclick_inside = event.is_local;
        }
        invalidate();
        return Widget::on_mouse_click(event);
    }

    virtual auto draw() -> void {
        auto color = content.background_color;
        content.fill(color);

        if (mouse_over) {
            color = Lighter(content.background_color, 0.1);
        }

        content.fill(color);

        if (mouse_over) {
            auto str = fmt::format("{} Position = {}x{} ", state_pressed ? "*" : " ", pos.x, pos.y);
            content.write_fixed(Position{4, 4}, str, MakeColor(0xf, 0xf, 0));
        } else {
            if (state_pressed)
                content.write_fixed_char(Position{4, 4}, '*', MakeColor(0x0, 0x0, 0));
        }

        content.line(10, 23, 30, 23, 0);
        content.line(10, 24, 30, 24, 0xffffff);
        content.line(10, 25, 30, 25, 0);
        content.line(10, 26, 30, 26, 0xffffff);

        //     content.line(0, 0, content.size.width-1, 0, 0);

        content.line(10, 23, 70, 27, 0xff3388);
        content.draw_bezier(20, 2, 40, 36, 160, 37, 0x2f2af8);
        content.line_thikness(5, 23, 175, 37, 5, 0x9933fe);
        content.draw_rounded_rectangle(10, 5, 150, 30, 10, 0xffaaaa, 0xffaaaa);
    }
};

int main() {
    int timer_count = 0;
    auto platform = ThePlatform();
    platform.init();
    //    platform.default_theme = std::make_shared<ThemeVision>(0x00ff00);
    //    platform.default_theme = std::make_shared<ThemeVision>();
    //    platform.default_theme = std::make_shared<ThemeRedmond>();
    //    platform.default_theme = std::make_shared<ThemePlasma>(0xff6737);
    platform.default_theme = std::make_shared<ThemePlasma>();

    Timer t1(500, true, [&timer_count]() {
        timer_count++;
        //        spdlog::info("timer");
    });

    //    auto w2 = platform.open_window(300, 300, 640, 480, "test 2");
    //    w2->content.background_color = 0x00FF00;
    //    platform.show_window(w2);

    auto w1 = platform.open_window(100, 100, 640, 480, "test 1");
    //    w1->add_new<Label>(Position{10, 10}, Size{100, 20}, "Hello world!");
    //    w1->add_new<TextField>(Position{10, 35}, Size{165, 30});

    std::vector<std::string> options = {
        "Option 1",
        "Option 2",
        "Option 3",
        "Option 4",
    };

    auto ll = w1->add_new<ListView>(Position{10, 70}, Size{165, 100});
    ll->adapter = std::make_shared<ListItemAdapter>(options);
    /*
        auto rb = w1->add_new<RadioButtonGroup>(Position{400, 20}, 160, options);
        rb->on_selected = [](int index, Checkbox &button) {
            spdlog::info("Selected item {} with text {}", index, button.text);
        };
        rb->radio_buttons[1]->is_enabled = false;

        auto debug_widget = w1->add_new<DebugWidget>(Position{400, 160}, Size{200, 40}, 0x22dd37);
        auto cb = w1->add_new<Checkbox>(Position{400, 130}, 220, "Show/hide debug widget");
        cb->on_checkbox_change = [debug_widget](Checkbox &cb) {
            if (cb.is_checked) {
                debug_widget->show();
            } else {
                debug_widget->hide();
            }
        };
        cb->set_checked(EventPropagation::handled);

        w1->add_new<ScrollBar>(Position{615, 00}, 480, false)->set_values(100, 200, 200, 5);

        std::shared_ptr<ScrollBar> scroll;
        std::shared_ptr<Spinbox> spin;

        scroll = w1->add_new<ScrollBar>(Position{10, 380}, 400, true);
        scroll->set_values(1000, 2000, 200, 50);
        scroll->did_change = [&spin](auto scrollbar, auto value) { spin->set_value(value); };
        spin = w1->add_new<Spinbox>(Position{10, 330}, Size{165, 30});
        spin->set_values(1000, 2000, 200);
        spin->did_change = [&scroll](auto spinbox, auto value) { scroll->set_value(value); };

        w1->add_new<Button>(Position{10, 420}, Size{200, 40}, "OK", true, [&platform]() {
            spdlog::info("OK clicked!");
            platform.exit_loop = true;
        });
        w1->add_new<Button>(Position{220, 420}, Size{200, 40}, "Cancel", false, [&platform]() {
              static auto clicked_count = 0;
              clicked_count++;
              spdlog::info("Cancel clicke1d! count = {}", clicked_count);
          })->set_auto_repeat(300, 700);
    */
    platform.show_window(w1);

    t1.start();
    platform.main_loop();
    platform.done();
    t1.stop();
    spdlog::info("Timer run for {} times", timer_count);
    return 0;
}
