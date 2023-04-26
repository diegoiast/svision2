#pragma once

#include <platform.h>

class PlatformWin32 : public Platform
{
public:
    PlatformWin32();
    virtual auto init() -> void override;
    virtual auto done() -> void override;
    virtual auto open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow> override;
    virtual auto show_window(std::shared_ptr<PlatformWindow> window) -> void override;
    virtual auto main_loop() -> void override;
};

using ThePlatform = PlatformWin32;
