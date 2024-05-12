#pragma once

#include <widget.h>

struct ImageView : Widget {
    Bitmap image;

    ImageView(const Bitmap &image);
    virtual auto draw() -> void override;
};
