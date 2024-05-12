#include <imageview.hpp>

ImageView::ImageView(const Bitmap &image) {
    this->draw_background = false;
    this->image.copy_from(image);
}

auto ImageView::draw() -> void {
    this->content.fill(0);
    this->content.draw({0, 0}, image);
}
