#pragma once

#include <bitmap.h>
#include <widget.h>
#include <events.h>

#include <string>
#include <memory>
#include <list>


class Platform
{
public:
    bool exit_loop = false;

    bool close_on_last_window = true;
    virtual auto init() -> void = 0;
    virtual auto done() -> void = 0;
    virtual auto open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow> = 0;
    virtual auto main_loop() -> void = 0;
};
