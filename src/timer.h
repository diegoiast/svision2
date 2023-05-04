/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>
#include <functional>
#include <signal.h>

struct PlatformTimer {
    int64_t millies = 0;
    bool repeating = false;
    bool is_running = false;
    std::function<void()> callback;

    PlatformTimer(int64_t millies, bool repeating, std::function<void()> callback) {
        this->millies = millies;
        this->repeating = repeating;
        this->callback = callback;
    }
    PlatformTimer() {};
    virtual ~PlatformTimer() {};

    virtual auto initialize() -> void = 0;
    virtual auto start() -> void = 0;
    virtual auto stop() -> void = 0;
};

#if defined(POSIX) || defined(__linux__)
struct PosixTimer : PlatformTimer {
    PosixTimer(int64_t millies, bool repeating, std::function<void()> callback);
    PosixTimer() {};
    ~PosixTimer();

    virtual auto initialize() -> void override;
    virtual auto start() -> void override;
    virtual auto stop() -> void override;

  private:
    static auto handler(sigval sival_int) -> void;
    timer_t timerid = 0;
};

using Timer = PosixTimer;
#endif

#if defined(_win32) || defined(WIN32)

#include <windef.h>
#include <winuser.h>
#include <map>

struct Win32Timer : PlatformTimer {
    Win32Timer(int64_t millies, bool repeating, std::function<void()> callback);
    Win32Timer() {};
    ~Win32Timer();

    virtual auto initialize() -> void override;
    virtual auto start() -> void override;
    virtual auto stop() -> void override;

  private:
    UINT_PTR timer_id = 0;

//    static auto handler(sigval sival_int) -> void;
//    timer_t timerid = 0;
    static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    static std::map<UINT_PTR , Win32Timer*> available_timers;
};
using Timer = Win32Timer;
#endif

