#pragma once

#include "image.hpp"

#include <vector>
#include <string>

namespace cord {

enum class TextureWrap {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
};

enum class TextureFilter {
    NEAREST,
    LINEAR
};

struct TextureParams {
    TextureWrap wrapS = TextureWrap::REPEAT;
    TextureWrap wrapT = TextureWrap::REPEAT;

    TextureFilter minFilter = TextureFilter::LINEAR;
    TextureFilter magFilter = TextureFilter::LINEAR;

    const static TextureParams NEAREST;

    const static TextureParams LINEAR;

    static unsigned int getGLWrap(TextureWrap wrap);

    static unsigned int getGLFilter(TextureFilter filter);

    void enable() const;
};

class Texture {
public:
    ~Texture();

    int getWidth() const;
    int getHeight() const;
    int getChannelCount() const;

    static Texture* create(Image* image);

    static Texture* create(Image* image, TextureParams params);

    static Texture* create(unsigned int texture, unsigned int width, unsigned int height, unsigned int channelCount);

    static Texture* load(const char* path);

    static Texture* load(const char* path, TextureParams params);

    void bind(unsigned int unit = 0) const;

    static void init();

    static void unbind(unsigned int unit = 0);

    static void unbindAll();

    static Texture* EMPTY;

private:
    Texture();

private:
    static std::vector<Texture*> loadedTextures;

private:
    unsigned int texture;

    int width;
    int height;
    int channels;

    std::string path;
};

}