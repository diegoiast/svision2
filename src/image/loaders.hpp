/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "bitmap.h"
#include <memory>
#include <string>

struct ImageDecoder {
    virtual auto decode(const std::string &filename) -> std::shared_ptr<Bitmap> = 0;
    virtual ~ImageDecoder() = default;
};

struct ImageLoader {
    ImageLoader();
    ~ImageLoader();

    auto registerDecoder(std::unique_ptr<ImageDecoder> decompressor) -> void;
    auto loadFile(const std::string &filename) -> std::shared_ptr<Bitmap>;

  private:
    std::vector<std::unique_ptr<ImageDecoder>> decoders;
};
