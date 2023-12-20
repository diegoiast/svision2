/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "fontproviderfreetype.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <string_view>

// #include <ft2build.h>

FontProviderFreetype::FontProviderFreetype(const std::string_view default_font) {
    if (FT_Init_FreeType(&library)) {
        spdlog::error("Freetype: Could not initialize");
        return;
    }

    auto error = FT_New_Face(library, default_font.data(), 0, &face);
    if (error) {
        spdlog::error("Freetype: Could not load font: {}, {}", default_font.data(),
                      FT_Error_String(error));
        FT_Done_FreeType(library);
        return;
    }

    initialized = true;
}

// Function to extract one Unicode code point from a UTF-8 string_view
// Returns the extracted Unicode code point and updates the iterator
int32_t extractUnicodeCharacter(std::string_view::const_iterator &it,
                                const std::string_view::const_iterator &end) {
    int32_t unicodeChar = 0;
    size_t bytesRead = 0;

    // Decode the Unicode character
    char32_t firstByte = static_cast<unsigned char>(*it);
    if (firstByte <= 0x7F) {
        unicodeChar = firstByte;
        bytesRead = 1;
    } else if ((firstByte & 0xE0) == 0xC0) {
        unicodeChar = (firstByte & 0x1F) << 6;
        unicodeChar |= (static_cast<unsigned char>(*(it + 1)) & 0x3F);
        bytesRead = 2;
    } else if ((firstByte & 0xF0) == 0xE0) {
        unicodeChar = (firstByte & 0x0F) << 12;
        unicodeChar |= (static_cast<unsigned char>(*(it + 1)) & 0x3F) << 6;
        unicodeChar |= (static_cast<unsigned char>(*(it + 2)) & 0x3F);
        bytesRead = 3;
    } else if ((firstByte & 0xF8) == 0xF0) {
        unicodeChar = (firstByte & 0x07) << 18;
        unicodeChar |= (static_cast<unsigned char>(*(it + 1)) & 0x3F) << 12;
        unicodeChar |= (static_cast<unsigned char>(*(it + 2)) & 0x3F) << 6;
        unicodeChar |= (static_cast<unsigned char>(*(it + 3)) & 0x3F);
        bytesRead = 4;
    } else {
        // Handle invalid UTF-8 encoding
        //        std::cerr << "Invalid UTF-8 encoding\n";
    }

    // Update the iterator
    it += bytesRead;

    return unicodeChar;
}

static auto draw_glyph(Bitmap &bitmap, int x, int y, uint32_t color, FT_GlyphSlot slot) {
    for (int dy = 0; dy < slot->bitmap.rows; dy++) {
        for (auto dx = 0; dx < slot->bitmap.width; dx++) {
            auto glyphColor = slot->bitmap.buffer[dy * slot->bitmap.width + dx];
            bitmap.blend_pixel(x + dx, y + dy, color, glyphColor);
        }
    }
}

auto FontProviderFreetype::write(Bitmap &bitmap, Position position, const std::string_view text,
                                 const uint32_t color) -> void {
    if (!initialized) {
        return;
    };

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    auto text_bounds = text_size(text);
    if (debug_render) {
        bitmap.draw_rectangle(position.x, position.y, text_bounds.width, text_bounds.height,
                              0x00ff00, 0x00ff00);
        auto yyy = position.y + (face->size->metrics.ascender >> 6);
        bitmap.line(position.x, yyy, position.x + text_bounds.width, yyy, 0xff8080);
    }

    auto penX = position.x * 64;
    auto penY = position.y * 64;
    auto it = text.begin();
    const auto end = text.end();

    while (it != end) {
        auto code_point = extractUnicodeCharacter(it, end);
        auto error = FT_Load_Char(face, code_point, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL);
        if (error) {
            //            spdlog::error("Freetype: Error rendering glyph: {}\n",
            //            FT_Error_String(error));
            continue;
        }

        // https://stackoverflow.com/questions/62374506/how-do-i-align-glyphs-along-the-baseline-with-freetype
        // https://freetype.org/freetype2/docs/tutorial/step2.html
        // https://kevinboone.me/fbtextdemo.html?i=2

        auto physicallStartX = (penX + face->glyph->bitmap_left) >> 6;
        auto physicallStartY = (penY - face->glyph->bitmap_top) >> 6;
        auto delta = text_bounds.height - face->glyph->bitmap_top;

        draw_glyph(bitmap, physicallStartX, physicallStartY + delta, color, face->glyph);

        penX += face->glyph->advance.x;
        penY += face->glyph->advance.y;
    }
}

auto FontProviderFreetype::text_size(const std::string_view text) -> Size {
    if (!initialized) {
        return {0, 0};
    };

    auto penX = 0;
    auto penY = (unsigned int)0;
    auto it = text.begin();
    const auto end = text.end();

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    while (it != end) {
        auto code_point = extractUnicodeCharacter(it, end);
        auto error = FT_Load_Char(face, code_point, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL);
        if (error) {
            continue;
        }

        FT_GlyphSlot slot = face->glyph;
        penX += slot->advance.x;
        penY += slot->advance.y;
    }

    auto size_x = penX / 64;
    auto size_y = (penY + face->size->metrics.height) / 64;
    return {size_x, (int)size_y};
}
