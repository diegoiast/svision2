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
#include <label.h>
#include <spdlog/spdlog.h>

auto handle_event(std::shared_ptr<PlatformWindow> w, const Event &ev);
auto handle_event(int window_id, int widget_id, const Event &ev);

int main() {
    auto platform = ThePlatform();
    platform.init();

    auto w2 = platform.open_window(300, 300, 640, 480, "test 2");
    w2->background_color = 0x00FF00;
    platform.show_window(w2);

    auto w1 = platform.open_window(100, 100, 640, 480, "test 1");
    w1->add(std::make_shared<Widget>(Position{350, 10}, Size{200, 40}, 0x22dd37));
    w1->add(std::make_shared<Button>(Position{10, 100}, Size{200, 40}, "OK", true, [&platform]() {
        spdlog::info("OK clicked!");
        platform.exit_loop = true;
    }));
    w1->add(std::make_shared<Button>(Position{220, 100}, Size{200, 40}, "Cancel", false,
                                     [&platform]() { spdlog::info("Cancel clicke1d!"); }));
    w1->add(std::make_shared<Label>(Position{10, 10}, Size{100, 20}, "Hello world!"));
    platform.show_window(w1);

    platform.main_loop();
    platform.done();
    return 0;
}
