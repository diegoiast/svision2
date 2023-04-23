// https://zserge.com/posts/fenster/

#include "platformx11.h"

int main()
{
    auto platform = PlatformX11();
    platform.init();
    platform.close_on_last_window = true;
    auto w1 = platform.open_window(100, 100, 640, 480, "test 1");
    auto w2 = platform.open_window(300, 300, 640, 480, "test 2");
    platform.main_loop();
    platform.done();
    return 0;
}