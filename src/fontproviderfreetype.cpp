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

auto FontProviderFreetype ::write(Bitmap &bitmap, Position position, const std::string_view text,
                                  const uint32_t color) -> void {
    if (!initialized) {
        return;
    };

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    size_t strPos = 0;
    int penX = position.x * 64;
    int penY = position.y * 64;
    int baseline = face->size->metrics.ascender; // Get the baseline offset
    int capline = face->size->metrics.height;    // Get the capline offset

    auto it = text.begin();
    const auto end = text.end();

    while (it != end) {
        int32_t code_point = extractUnicodeCharacter(it, end);
        auto error = FT_Load_Char(face, code_point, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL);
        if (error) {
            //            spdlog::error("Freetype: Error rendering glyph: {}\n",
            //            FT_Error_String(error));
            continue;
        }

        FT_GlyphSlot slot = face->glyph;

        // https://stackoverflow.com/questions/62374506/how-do-i-align-glyphs-along-the-baseline-with-freetype
        // https://freetype.org/freetype2/docs/tutorial/step2.html
        //        bitmap.draw_rectangle(penX / 64, (penY + slot->bitmap_top) / 64,
        //        slot->bitmap.width,
        //                              slot->bitmap.rows, 0x00ff00, 0x00ff00);

        //        spdlog::info("Base line for {} is at {}", code_point, slot->bitmap_top);
        auto yyy = (penY + slot->bitmap.rows * 64 - slot->bitmap_top) / 64;
        auto dy = baseline - slot->bitmap.rows * 64;
        yyy += dy / 64;
        bitmap.line(penX / 64, yyy, penX / 64 + slot->bitmap.width, yyy, 0xff00ff);

        for (int y = 0; y < slot->bitmap.rows; y++) {
            for (int x = 0; x < slot->bitmap.width; x++) {
                int pixelX = penX + x * 64 + slot->bitmap_left;
                int pixelY = penY + y * 64 - slot->bitmap_top + dy;

                uint32_t glyphColor = slot->bitmap.buffer[y * slot->bitmap.width + x];
                if (glyphColor >= 127) {
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
    auto text_width = 0;
    auto text_height = 0;
    auto point_size = face->size->metrics.x_ppem;

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

        text_width += (int)((slot->advance.x * point_size) / face->units_per_EM);
    }

    // Get and convert ascender and descender values
    int ascender_pixels = (int)((face->size->metrics.ascender * point_size) / face->units_per_EM);
    int descender_pixels = (int)((face->size->metrics.descender * point_size) / face->units_per_EM);

    // Calculate total text height
    text_height = ascender_pixels + descender_pixels;

    spdlog::info("Size for {} is {}x{}", text, text_width, text_height);
    return {text_width, text_height};
}
