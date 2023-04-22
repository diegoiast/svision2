#pragma once

#include <cstdlib>
#include <cstdint>

namespace svision2 {

using color = int32_t;

class surface {
public:
    virtual ~surface() {};
    virtual size_t width() = 0;
    virtual size_t height() = 0;

    virtual void setPixel(size_t x, size_t y, svision2::color color) = 0;
    virtual svision2::color getPixel(size_t x, size_t y) = 0;
};

}
