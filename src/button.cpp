#include <button.h>
#include <spdlog/spdlog.h>

Button::Button(Position pp, Size size, std::string text)
    : Widget(pp, size, 0)
{
    this->text = text;
}

auto Button::draw() -> void
{
    auto background_color = MakeColor(0x80, 0x80, 0x80);
    auto line1 = MakeColor(0xe0, 0xe0, 0xe0);
    auto line2 = MakeColor(0x80, 0x80, 0x80);
    auto offset = 0;

    switch (state)
    {
    case States::Normal:
        spdlog::info("Drawing normal button");
        line1 = MakeColor(0xe0, 0xe0, 0xe0);
        line2 = MakeColor(0x80, 0x80, 0x80);
        break;
    case States::Hovered:
        spdlog::info("Drawing Hovered button");
        // line1 = MakeColor(0x00, 0xff, 0x00);
        // line2 = MakeColor(0x00, 0xff, 0x00);
        background_color = MakeColor(0x90, 0x90, 0x90);
        break;
    case States::ClickedUnreleased:
        spdlog::info("Drawing ClickedUnreleased button");
        line2 = MakeColor(0xe0, 0xe0, 0xe0);
        line1 = MakeColor(0x80, 0x80, 0x80);
        background_color = MakeColor(0x90, 0x90, 0x90);
        offset = 1;
        break;

    default:
        break;
    }

    content.line(0, 0, 0, content.size.height - 1, line1);
    content.line(0, 0, content.size.width - 1, 0, line1);
    content.line(content.size.width - 1, 0, content.size.width - 1, content.size.height - 1, line2);
    content.line(0, content.size.height - 1, content.size.width - 1, content.size.height - 1, line2);
    // spdlog::info("Background color {0:x}", background_color);
    content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background_color);
    content.write_fixed(Position{7+offset, 7+offset}, text, 0);
    content.write_fixed(Position{5+offset, 5+offset}, text, 0xffffff);
}

auto Button::on_hover(const EventMouse &event) -> void
{
}

auto Button::on_mouse_enter() -> void
{
    if (state == States::ClickedUnreleased)
    {
        // spdlog::info("Mid click - not changing to hover");
        return;
    }
    state = States::Hovered;
    needs_redraw = true;
    spdlog::info("New state: hover");
}

auto Button::on_mouse_leave() -> void
{
    state = States::Normal;
    needs_redraw = true;
    spdlog::info("New state: normal");
}

auto Button::on_mouse_click(const EventMouse &event) -> void
{
    if (event.type == MouseEvents::Press)
    {
        state = States::ClickedUnreleased;
        needs_redraw = true;
        spdlog::info("New state: clicked-unreleased");
    }
    else if (event.type == MouseEvents::Release && event.is_local)
    {
        state = States::Normal;
        spdlog::info("New state: normal");
        if (event.is_local)
        {
            spdlog::info("Clicked on button");
        }
        else
        {
            spdlog::info("Clicked aborted");
        }
        needs_redraw = true;
    }
}
