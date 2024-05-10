#include <fstream>
#include <iostream>
#include <iterator>
#include <string_view>

#include <spdlog/spdlog.h>

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "fontproviders/fontproviderstb.hpp"

static std::vector<unsigned char> readFile(std::string_view filename) {
    // TODO handle errors
    std::ifstream file(filename.data(), std::ios::binary);
    file.unsetf(std::ios::skipws);
    std::streampos fileSize;
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> vec;
    vec.reserve(fileSize);
    vec.insert(vec.begin(), std::istream_iterator<unsigned char>(file),
               std::istream_iterator<unsigned char>());

    return vec;
}

// Function to extract one Unicode code point from a UTF-8 string_view
// Returns the extracted Unicode code point and updates the iterator
static int32_t extractUnicodeCharacter(std::string_view::const_iterator &it) {
    int32_t unicodeChar = 0;
    size_t bytesRead = 0;

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
    }

    it += bytesRead;
    return unicodeChar;
}

FontProviderSTB::FontProviderSTB(const std::string_view default_font) {
    font_data = readFile(default_font.data());
    auto data = font_data.data();
    auto ret = stbtt_InitFont(&this->font_info, data, stbtt_GetFontOffsetForIndex(data, 0));
    spdlog::warn("initfont = {}", ret);
}

void FontProviderSTB::write(Bitmap &bitmap, Position position, const std::string_view text,
                            uint32_t color) {
    int ascent, descent, lineGap;
    auto x = position.x;
    auto y = position.y;

    float scale = stbtt_ScaleForPixelHeight(&font_info, static_cast<float>(fontSize));
    stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &lineGap);
    ascent = ascent * scale;

    if (debug_render) {
        auto text_bounds = text_size(text);
        bitmap.draw_rectangle(position.x, position.y, text_bounds.width, text_bounds.height,
                              0x00ff00, 0x00ff00);
        auto yyy = position.y + (ascent);
        bitmap.line(position.x, yyy, position.x + text_bounds.width, yyy, 0xff8080);
    }

    auto it = text.begin();
    const auto end = text.end();
    auto last_point = 0;
    while (it != end) {
        auto code_point = extractUnicodeCharacter(it);
        auto advance = 0;
        auto lsb = 0;
        auto glyphWidth = 0;
        auto glyphHeight = 0;
        auto gx = 0;
        auto gy = 0;

        if (last_point != 0) {
            auto kern = stbtt_GetCodepointKernAdvance(&font_info, last_point, code_point);
            x += roundf(kern * scale);
        }

        auto bitmapBuffer = static_cast<unsigned char *>(nullptr);
        auto glyphIndex = stbtt_FindGlyphIndex(&font_info, code_point);
        stbtt_GetCodepointHMetrics(&font_info, code_point, &advance, &lsb);
        bitmapBuffer = stbtt_GetGlyphBitmap(&font_info, scale, scale, glyphIndex, &glyphWidth,
                                            &glyphHeight, &gx, &gy);

        if (bitmapBuffer) {
            for (auto yy = 0; yy < glyphHeight; ++yy) {
                for (auto xx = 0; xx < glyphWidth; ++xx) {
                    auto index = yy * glyphWidth + xx;
                    if (bitmapBuffer[index] > 10) {
                        auto targetX = x + xx + gx;
                        auto targetY = y + yy + gy + ascent;
                        bitmap.blend_pixel(targetX, targetY, color, bitmapBuffer[index]);
                    }
                }
            }
        }
        x += static_cast<int>(advance * scale);
        last_point = code_point;
    }
}

auto FontProviderSTB::text_size(const std::string_view text) -> Size {
    auto metrics = Size{};
    auto ascent = 0;
    auto descent = 0;
    auto lineGap = 0;
    auto x0 = 0;
    auto y0 = 0;
    auto x1 = 0;
    auto y1 = 0;

    stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &lineGap);
    auto scale = stbtt_ScaleForPixelHeight(&font_info, (float)fontSize);
    auto baseline = (int)(ascent * scale);
    auto last_point = 0;
    auto it = text.begin();
    auto const end = text.end();

    while (it != end) {
        auto code_point = extractUnicodeCharacter(it);
        if (last_point != 0) {
            auto kern = stbtt_GetCodepointKernAdvance(&font_info, last_point, code_point);
            x1 += roundf(kern * scale);
        }
        if (code_point == '\n') {
            y0 -= (ascent - descent + lineGap) * scale;
            continue;
        }
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&font_info, code_point, &advance, &lsb);
        x1 = x0 + advance * scale;
        y1 = y0 + baseline;
        x0 = x1;
    }

    metrics.width = x1;
    metrics.height = y1;
    return metrics;
};
