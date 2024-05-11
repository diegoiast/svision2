#include <image/loaders.hpp>
#include <spdlog/spdlog.h>


// keep this local to this TU
namespace {

#if defined(SVISION_USE_STB)
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

struct ImageDecoderSTB : public ImageDecoder {
    auto decode(const std::string &filename, Bitmap &bitmap) -> bool  {
        int width, height, channels;
        unsigned char *data =
            stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (data) {
            spdlog::info("Decoding {} using STB decoder...", filename);
            bitmap.size = {width, height};
            bitmap.buffer.assign(data, data + width * height);
            stbi_image_free(data);
            return true;
        } else {
            spdlog::error("Failed to decode {} using STB decoder", filename);
            return false;
        }
    }
};
#endif

} // namespace


ImageLoader::ImageLoader()
{
#if defined(SVISION_USE_STB)
    registerDecoder(std::make_unique<ImageDecoderSTB>());
#endif
}

ImageLoader::~ImageLoader()
{

}

auto ImageLoader::registerDecoder(std::unique_ptr<ImageDecoder> decompressor) -> void {
    decoders.push_back(std::move(decompressor));
}

auto ImageLoader::loadFile(const std::string &filename, Bitmap &bitmap) -> bool{
    for (const auto &decoder : decoders) {
        if (decoder->decode(filename, bitmap)) {
            return true;
        }
    }
    return false;
}
