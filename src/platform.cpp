/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <spdlog/spdlog.h>

#include "image/loaders.hpp"
#include "platform.h"
#include "theme.h"

#if defined(SVISION_USE_FREETYPE)
#include "fontproviderfreetype.h"
#elif defined(SVISION_USE_STB)
#include "fontproviders/fontproviderstb.hpp"
#endif

void Platform::init()
{
    platform_init();

    if (!this->default_font) {
#if defined(SVISION_USE_FREETYPE)
        this->default_font = std::make_shared<FontProviderFreetype>(default_font_file);
#elif defined(SVISION_USE_STB)
        this->default_font = std::make_shared<FontProviderSTB>(default_font_file);
#endif
    }

    // TODO - detect GTK and use a GTK theme
    if (!this->default_theme) {
        default_theme = std::make_shared<ThemePlasma>(*this->default_font);
    }

    if (!this->image_loader){
        image_loader = std::make_shared<ImageLoader>();
    }
}
