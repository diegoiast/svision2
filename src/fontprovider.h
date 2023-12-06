/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <bitmap.h>
#include <string>

struct FontProvider {
    auto virtual write(Bitmap &, Position, const std::string_view, const uint32_t color)
        -> void = 0;
    auto virtual text_size(const std::string_view str) -> Size = 0;
};

struct FontProviderFixed : FontProvider {
    auto virtual write(Bitmap &, Position, const std::string_view, const uint32_t color)
        -> void override;
    auto virtual text_size(const std::string_view str) -> Size override;
};
