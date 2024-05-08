#include <button.h>
#include <layout.h>
#include <toolbar.hpp>

Toolbar::Toolbar() {
    this->layout = std::make_shared<HorizontalLayout>();
    this->layout->padding.set_horizontal(5);
    this->padding_style = PaddingStyle::TabHeader;
}

auto Toolbar::size_hint() const -> Size {
    auto padding_y = this->get_padding().get_vertical();
    auto hint = this->get_theme()->font->text_size("X");
    hint.width = 0;
    hint.height += padding_y;
    return hint;
}

auto Toolbar::add_shortcut(P<Shortcut> s) -> P<Toolbar> {
    auto b = add_new<Button>(s->title, false,
                             [s](auto &) {
                                 if (s->on_trigger) {
                                     s->on_trigger(*s, s->id);
                                 }
                             })
                 ->set_has_frame(false)
                 ->set_auto_shrink(true);
    return std::static_pointer_cast<Toolbar>(this->shared_from_this());
}
