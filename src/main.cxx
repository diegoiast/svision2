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
#include <combobox.h>
#include <image/loaders.hpp>
#include <imageview.hpp>
#include <label.h>
#include <listview.h>
#include <radiobuttongroup.h>
#include <scrollbar.h>
#include <spinbox.h>
#include <stackwidget.h>
#include <tabheader.h>
#include <tabwidget.h>
#include <textfield.h>

#include <fontproviders/fontproviderstb.hpp>
#include <themes/fluent.h>

#include <spdlog/spdlog.h>
#include <timer.h>

// TODO  future expansion, ideas
// auto handle_event(std::shared_ptr<PlatformWindow> w, const Event &ev);
// auto handle_event(int window_id, int widget_id, const Event &ev);

struct DebugWidget : public Widget {
    bool state_pressed = false;
    Position pos = {0, 0};
    bool unclick_inside = false;

    DebugWidget(uint32_t color) : Widget(color) { this->mouse_cursor = MouseCursor::Cross; }

    auto on_hover(const EventMouse &event) -> void {
        pos.x = event.x;
        pos.y = event.y;
        invalidate();
    }

    auto on_mouse_enter() -> void {
        mouse_over = true;
        invalidate();
    }

    auto on_mouse_leave() -> void {
        mouse_over = false;
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
            theme->font.write(content, {4, 4}, str, MakeColor(0xf, 0xf, 0));
        } else {
            if (state_pressed)
                theme->font.write(content, {4, 4}, "*", MakeColor(0xf, 0xf, 0));
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
    //    platform.default_font = std::make_shared<FontProviderSTB>(platform.default_font_file);
    //    platform.default_theme = std::make_shared<ThemeFluent>(*platform.default_font);
    //    platform.default_theme = std::make_shared<ThemeRedmond>(*platform.default_font);
    //    platform.default_theme = std::make_shared<ThemePlasma>(*platform.default_font, 0xff6737);
    //    platform.default_theme = std::make_shared<ThemePlasma>(*platform.default_font);
    platform.init();

    Timer t1(500, true, [&timer_count]() {
        timer_count++;
        //        spdlog::info("timer");
    });

#if 1
    auto w2 = platform.open_window(300, 300, 640, 480, "test 2");
    w2->main_widget.layout->padding.set_vertical(2);

    auto list = w2->add_new<Combobox>(std::vector<std::string_view>());
    auto tabs = w2->add_new<TabWidget>();
    auto bitmap = Bitmap();

    platform.image_loader->loadFile("vampire-riding-a-dinozaur.png", bitmap);
    //    platform.image_loader->loadFile("watering-can-garden-nature-5346272.jpg", bitmap);

    tabs->add_new_tab<ImageView>("Tab 1", bitmap);
    tabs->add_new_tab<Label>("Tab with long name", "This is the second widget")
        ->content.background_color = MakeColor(0xaa, 0x22, 0x22);
    tabs->add_new_tab<Label>("Tab 3", "This is the third widget")->content.background_color =
        MakeColor(0x33, 0xaa, 0x22);
    tabs->add_new_tab<Label>("Tab 4", "This is the forth widget")->content.background_color =
        MakeColor(0x33, 0x22, 0xaa);

    list->set_items(tabs->page_names());
    list->on_item_selected = [&tabs](auto &, auto index) {
        // propagate event to the tabs
        tabs->set_active_page(index);
    };
    tabs->on_page_selected = [&list](auto &, auto index) {
        // propagate event to the combo box
        list->set_active_index(index);
    };

    auto b1 = std::make_shared<Button>("+")
                  ->set_on_click([&tabs](auto &button) {
                      tabs->add_new_tab<Label>("Tab", "This is a dynamic tab");
                      MakeColor(0x33, 0xaa, 0x22);
                  })
                  ->set_auto_shrink(true)
                  ->set_has_frame(false);
    auto b2 = std::make_shared<Button>("x")
                  ->set_on_click([&tabs](auto &button) {
                      auto active_tab = tabs->get_active_tab();
                      tabs->remove_page(active_tab);
                  })
                  ->set_auto_shrink(true)
                  ->set_has_frame(false);
    tabs->set_buttons(b1, b2);

    platform.show_window(w2);
#endif

#if 1
    auto w1 = platform.open_window(100, 100, 640, 480, "test 1");
    auto l = w1->add_new<Label>("test 1 - Hello world! glqi שלום עולם");
    l->frame = {FrameStyles::Normal, FrameSize::SingleFrame};
    l->weight = 0.3;
    w1->add_new<TextField>();

    auto center_layout = w1->main_widget.layout->add(std::make_shared<HorizontalLayout>());
    auto l_left = center_layout->add(std::make_shared<VerticalLayout>());
    auto l_right = center_layout->add(std::make_shared<VerticalLayout>());

    w1->main_widget.layout->margin.set(5);
    center_layout->padding.set_horizontal(10);

    //    l_right->margin.set(5);
    w1->add_new_to_layout<ListView>(l_left)->adapter =
        std::make_shared<ListItemAdapter>(std::vector<std::string_view>{
            "Option 1 (default)",
            "Option 2",
            "Option 3",
            "Option 4",
            "Option 5",
            "Option 6",
            "Option 7",
            "Option 8",
            "Option 9",
            "Option 10",
            "Option 11",
            "Option 12 (last)",
        });
    auto rb = w1->add_new_to_layout<RadioButtonGroup>(l_right, std::vector<std::string>{
                                                                   "Option 1",
                                                                   "Option 2",
                                                                   "Option 3",
                                                                   "Option 4",
                                                               });
    rb->on_selected = [](int index, Checkbox &button) {
        spdlog::info("Selected item {} with text {}", index, button.text);
    };
    rb->radio_buttons[1]->is_enabled = false;
    w1->add_new_to_layout<Combobox>(l_right, std::vector<std::string_view>{
                                                 "Spring",
                                                 "Summer",
                                                 "Autumn/Fall",
                                                 "Winter",
                                             });

    auto debug_widget = w1->add_new_to_layout<DebugWidget>(l_right, 0x22dd37);
    auto cb = w1->add_new_to_layout<Checkbox>(l_right, "Show/hide debug widget");
    cb->on_checkbox_change = [debug_widget](const Checkbox &cb) {
        if (cb.is_checked) {
            debug_widget->show();
        } else {
            debug_widget->hide();
        }
        // this is to trigger re-layout
        debug_widget->window->relayout();
    };
    cb->set_checked(EventPropagation::handled);
    debug_widget->weight = 0.5;

    std::shared_ptr<ScrollBar> scroll;
    std::shared_ptr<Spinbox> spin;

    scroll = w1->add_new<ScrollBar>(true);
    scroll->set_values(1000, 2000, 200, 50);
    scroll->did_change = [&spin](auto scrollbar, auto value) { spin->set_value(value); };
    spin = w1->add_new<Spinbox>();
    spin->set_values(1000, 2000, 200);
    spin->did_change = [&scroll](auto spinbox, auto value) { scroll->set_value(value); };

    auto buttons_layout = w1->main_widget.layout->add(std::make_shared<HorizontalLayout>());
    buttons_layout->margin.set_horizontal(5);
    buttons_layout->margin.set_vertical(5);
    buttons_layout->padding.set_horizontal(10);
    buttons_layout->add(std::make_shared<HorizontalSpacer>())->weight = 2;

    w1->add_new_to_layout<Button>(buttons_layout, "OK", true, [&platform](auto &button) {
        spdlog::info("OK clicked!");
        platform.exit_loop = true;
    });

    std::shared_ptr<Button> cancel_button = w1->add_new_to_layout<Button>(
        buttons_layout, "Cancel", false, [&cancel_button](auto &button) {
            static auto clicked_count = 0;
            static auto text = std::string();

            clicked_count++;
            text = fmt::format("Cancel ({})", clicked_count);
            spdlog::info("Cancel Clicked! count = {}", clicked_count);
            cancel_button->text = text;
            cancel_button->invalidate();
        });
    cancel_button->set_auto_repeat(300, 700);
    platform.show_window(w1);
#endif

    t1.start();
    platform.main_loop();
    platform.done();
    t1.stop();
    spdlog::info("Timer run for {} times", timer_count);
    return 0;
}
