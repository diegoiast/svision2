#include <button.h>
#include <image/loaders.hpp>
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

auto Toolbar::add_shortcut(int id, std::string_view title, std::string_view icon,
                           std::function<void(Shortcut &, int)> callback)
    -> std::shared_ptr<Toolbar> {
    auto s = std::make_shared<Shortcut>(id, title, icon, callback);
    return add_shortcut(s);
}

auto Toolbar::add_shortcut(std::shared_ptr<Shortcut> s) -> std::shared_ptr<Toolbar> {
    auto icon = std::shared_ptr<Bitmap>();
    auto b = add_new<Button>(s->title, false,
                             [s](auto &) {
                                 if (s->on_trigger) {
                                     s->on_trigger(*s, s->id);
                                 }
                             })
                 ->set_has_frame(false)
                 ->set_can_focus(false)
                 ->set_auto_shrink(true);
    if ((icon = window->platform->image_loader->loadFile(s->icon_name))) {
        b->set_icon(icon);
    }
    return std::static_pointer_cast<Toolbar>(this->shared_from_this());
}
