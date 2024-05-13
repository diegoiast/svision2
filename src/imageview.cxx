/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <imageview.hpp>

ImageView::ImageView(const Bitmap &image) {
    this->draw_background = false;
    this->image.copy_from(image);
}

auto ImageView::on_resize() -> void {
    this->content.rescale_from(image, content.size);
    this->needs_redraw = true;
}
