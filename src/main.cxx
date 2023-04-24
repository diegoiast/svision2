// https://zserge.com/posts/fenster/

#include "platformx11.h"


auto handle_event(std::shared_ptr<PlatformWindow> w, const Event &ev);
auto handle_event(int window_id, int widget_id, const Event &ev);

int main()
{
    auto platform = PlatformX11();
    platform.init();
    platform.close_on_last_window = true;
    auto w1 = platform.open_window(100, 100, 640, 480, "test 1");

    // w1->content.fill(0, 0, 0, MakeColor(0x32, 0x77, 0x12));
    w1->content.background_color = 0x0000FF;
    w1->content.fill_rect(0, 0, 540, 380, MakeColor(0x32, 0x77, 0x12));
    w1->content.fill_circle(100, 100, 24, 0xFF8822);

    auto w2 = platform.open_window(300, 300, 640, 480, "test 2");
    w2->content.background_color = 0x00FF00;
    w2->content.fill_rect(0, 0, 440, 380,MakeColor(0xfa, 0xe7, 0xd3));
    // w2->content.fill(0, 0, 0, MakeColor(0xfa, 0xe7, 0xd3));
    w2->content.fill_rect(100, 100, 24, 30, 0x8822aa);

    platform.main_loop();
    platform.done();
    return 0;
}