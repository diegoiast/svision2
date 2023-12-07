/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "fontproviderfreetype.h"
#include "spdlog/spdlog.h"
#include <algorithm>

// #include <ft2build.h>

FontProviderFreetype::FontProviderFreetype() {
    if (FT_Init_FreeType(&library)) {
        spdlog::error("Freetype: Could not initialize");
        return;
    }

    auto error = FT_New_Face(library, font_file.data(), 0, &face);
    if (error) {
        spdlog::error("Freetype: Could not load font: {}, {}", font_file.data(),
                      FT_Error_String(error));
        FT_Done_FreeType(library);
        return;
    }

    initialized = true;
}

auto FontProviderFreetype ::write(Bitmap &bitmap, Position position, const std::string_view text,
                                  const uint32_t color) -> void {
    if (!initialized) {
        return;
    };

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    //    FT_Set_Char_Size(face, 0, fontSize * 64, 70, 70);

    size_t strPos = 0;
    int penX = position.x * 64;
    int penY = position.y * 64;

    while (strPos < text.length()) {
        size_t charPos = strPos + 1;
        while (charPos < text.length() && (text[charPos] & 0xC0) == 0x0) {
            charPos++;
        }

        auto utf8Char = text.substr(strPos, charPos - strPos);
        strPos = charPos;

        char ccc = utf8Char[0];
        //        auto index = FT_Get_Char_Index(face, ccc);
        auto index = ccc;
        auto error = FT_Load_Char(face, index, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL);
        if (error) {
            //            spdlog::error("Freetype: Error rendering glyph: {}\n",
            //            FT_Error_String(error));
            continue;
        }

        FT_GlyphSlot slot = face->glyph;
        for (int y = 0; y < slot->bitmap.rows; y++) {
            for (int x = 0; x < slot->bitmap.width; x++) {
                int pixelX = penX + x * 64;
                int pixelY = penY + y * 64 + slot->metrics.vertBearingY;

                uint32_t glyphColor = slot->bitmap.buffer[y * slot->bitmap.width + x];
                if (glyphColor > 127) {
                    bitmap.put_pixel(pixelX / 64, pixelY / 64, color);
                }
            }
        }

        penX += slot->advance.x;
        penY += slot->advance.y;
    }
}

auto FontProviderFreetype::text_size(const std::string_view text) -> Size {
    if (!initialized) {
        return {0, 0};
    };

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    size_t strPos = 0;
    long penX = 0;
    long penY = 0;

    while (strPos < text.length()) {
        size_t charPos = strPos + 1;
        while (charPos < text.length() && (text[charPos] & 0xC0) == 0x00) {
            charPos++;
        }

        auto utf8Char = text.substr(strPos, charPos - strPos);
        strPos = charPos;

        if (FT_Load_Char(face, FT_Get_Char_Index(face, utf8Char[0]),
                         FT_LOAD_RENDER | FT_LOAD_TARGET_MONO)) {
            // on error?
            continue;
        }

        FT_GlyphSlot slot = face->glyph;

        penX += slot->advance.x >> 6;
        penY += slot->advance.y >> 6;
    }

    spdlog::info("Size for {} is {}x{}", text, penX, penY);
    return {(int)penX, int(penY)};
}
