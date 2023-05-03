/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>
#include <signal.h>

struct Timer {
    std::function<void()> callback;
    int64_t millies = 0;
    bool repeating = false;
    bool is_running = false;

    Timer(int64_t millies, bool repeating, std::function<void()> callback);
    ~Timer();
    auto start() -> void;
    auto stop() -> void;

  private:
    static auto handler(sigval sival_int) -> void;
    timer_t timerid = 0;
};
