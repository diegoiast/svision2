/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <widget.h>

struct ImageView : Widget {
    Bitmap image;

    ImageView(const Bitmap &image);
    virtual auto on_resize() -> void override;
};
