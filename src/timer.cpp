/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "timer.h"
#include <cstring>
#include <errno.h>
#include <time.h>

Timer::Timer(int64_t millies, bool repeating, std::function<void()> callback) {
    this->millies = millies;
    this->repeating = repeating;
    this->callback = callback;
    initialize();
}

Timer::~Timer() {
    stop();
    timer_delete(timerid);
}

auto Timer::initialize() -> void {
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

auto Timer::start() -> void {
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

auto Timer::stop() -> void {
    if (!is_running) {
        return;
    }

    struct itimerspec trigger;
    memset(&trigger, 0, sizeof trigger);
    trigger.it_interval = {0, 0};
    trigger.it_value = {0, 0};

    auto ret = timer_settime(timerid, 0, &trigger, NULL);
    if (ret == -1) {
        // perror("setitimer error");
        exit(EXIT_FAILURE);
    }

    is_running = false;
}

auto Timer::handler(sigval sival_int) -> void {
    Timer *timer = static_cast<Timer *>(sival_int.sival_ptr);
    timer->callback();
}
