#include <image/loaders.hpp>
#include <spdlog/spdlog.h>

// keep this local to this TU
namespace {

#if defined(SVISION_USE_STB)
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

struct ImageDecoderSTB : public ImageDecoder {
    auto decode(const std::string &filename) -> std::shared_ptr<Bitmap> override {
        int width, height, channels;
        unsigned char *data =
            stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (data) {
            spdlog::info("Decoded {} using STB decoder...", filename);
            auto bitmap = std::make_shared<Bitmap>();
            bitmap->size = {width, height};
            size_t numPixels = width * height;
            for (size_t i = 0; i < numPixels; ++i) {
                uint32_t pixel = 0;
                for (int c = 0; c < 3; ++c) {
                    pixel |= static_cast<uint32_t>(data[i * 4 + c]) << (c * 8);
                }
                pixel |= static_cast<uint32_t>(data[i * 4 + 3]) << 24;
                bitmap->buffer.push_back(pixel);
            }

            stbi_image_free(data);
            return bitmap;
        } else {
            spdlog::error("Failed to decode {} using STB decoder", filename);
            return {};
        }
    }
};
#endif

} // namespace

ImageLoader::ImageLoader() {
#if defined(SVISION_USE_STB)
    registerDecoder(std::make_unique<ImageDecoderSTB>());
#endif
}

ImageLoader::~ImageLoader() {}

auto ImageLoader::registerDecoder(std::unique_ptr<ImageDecoder> decompressor) -> void {
    decoders.push_back(std::move(decompressor));
}

auto ImageLoader::loadFile(const std::string &filename) -> std::shared_ptr<Bitmap> {
    for (const auto &decoder : decoders) {
        auto bitmap = decoder->decode(filename);
        if (bitmap) {
            return bitmap;
        }
    }
    return {};
}
