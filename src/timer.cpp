/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "timer.h"
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <time.h>

#if defined(POSIX) || defined(__linux__)
PosixTimer::PosixTimer(int64_t millies, bool repeating, std::function<void()> callback) {
    this->millies = millies;
    this->repeating = repeating;
    this->callback = callback;
    initialize();
}

PosixTimer::~PosixTimer() {
    stop();
    timer_delete(timerid);
}

auto PosixTimer::initialize() -> void {
    struct sigevent sev;
    memset(&sev, 0, sizeof sev);
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = &Timer::handler;
    sev.sigev_value.sival_ptr = this;
    int ret = timer_create(CLOCK_MONOTONIC, &sev, &timerid);
    if (ret) {
        ret = errno;
        exit(1);
    }
}

auto PosixTimer::start() -> void {
    if (is_running) {
        stop();
    }

    struct itimerspec trigger;
    memset(&trigger, 0, sizeof trigger);
    trigger.it_value.tv_sec = millies / 1000;
    trigger.it_value.tv_nsec = (millies % 1000) * 1000000;
    if (repeating) {
        trigger.it_interval.tv_sec = trigger.it_value.tv_sec;
        trigger.it_interval.tv_nsec = trigger.it_value.tv_nsec;
    }
    auto ret = timer_settime(timerid, 0, &trigger, NULL);
    if (ret) {
        ret = errno;
        exit(1);
    }
    is_running = true;

    // TODO - single shot timers are broken, as the `is_running` flag does not
    // get cleared. I am unsure what this means on Posix timers.
}

auto PosixTimer::stop() -> void {
    if (!is_running) {
        return;
    }
    timer_delete(timerid);
    is_running = false;
}

auto PosixTimer::handler(sigval sival_int) -> void {
    PosixTimer *timer = static_cast<PosixTimer *>(sival_int.sival_ptr);
    timer->callback();
}
#endif

#if defined(_win32) || defined(WIN32)

std::map<UINT_PTR, Win32Timer *> Win32Timer::available_timers;

Win32Timer::Win32Timer(int64_t millies, bool repeating, std::function<void()> callback) {}
Win32Timer::~Win32Timer() { stop(); }

auto Win32Timer::initialize() -> void {}

auto Win32Timer::start() -> void {
    if (timer_id != 0) {
        stop();
    }

    timer_id = SetTimer(NULL, 1, millies, Win32Timer::TimerProc);
    Win32Timer::available_timers[timer_id] = this;
}

auto Win32Timer::stop() -> void {
    if (timer_id == 0) {
        return;
    }
    KillTimer(NULL, timer_id);
    Win32Timer::available_timers.erase(timer_id);
    timer_id = 0;
}

auto Win32Timer::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) -> void {
    auto instance = Win32Timer::available_timers[idEvent];
    instance->callback();
    if (!instance->repeating) {
        instance->stop();
    }
}
#endif
