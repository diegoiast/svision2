/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <fontprovider.h>

struct FontProviderFreetype : FontProvider {
    explicit FontProviderFreetype();

    auto virtual write(Bitmap &, Position, const std::string_view, const uint32_t color)
        -> void override;
    auto virtual text_size(const std::string_view str) -> Size override;

    FT_Library library;
    FT_Face face;
    bool initialized = false;
    const std::string_view font_file = "/usr/share/fonts/truetype/freefont/FreeSans.ttf";
    //    const std::string_view font_file = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    const int fontSize = 14;
};
