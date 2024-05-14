/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <fontprovider.h>
#include <stb/stb_truetype.h>

struct FontProviderSTB : FontProvider {
    explicit FontProviderSTB(const std::string_view default_font);

    auto virtual write(Bitmap &, Position, const std::string_view, const uint32_t color)
        -> void override;
    auto virtual text_size(const std::string_view str) -> Size override;

    stbtt_fontinfo font_info;
    std::vector<unsigned char> font_data;
    int fontSize = 16;
    bool debug_render = false;
};
