#include "texture.hpp"

#include <glad/glad.h>

using namespace cord;

Texture* Texture::EMPTY = nullptr;
std::vector<Texture*> Texture::loadedTextures;

const TextureParams TextureParams::LINEAR = {
    TextureWrap::REPEAT,
    TextureWrap::REPEAT,
    TextureFilter::LINEAR,
    TextureFilter::LINEAR
};

const TextureParams TextureParams::NEAREST = {
    TextureWrap::REPEAT,
    TextureWrap::REPEAT,
    TextureFilter::NEAREST,
    TextureFilter::NEAREST
};

unsigned int TextureParams::getGLWrap(TextureWrap wrap) {
    switch (wrap) {
    case TextureWrap::REPEAT:
        return GL_REPEAT;
    case TextureWrap::MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;
    case TextureWrap::CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;
    case TextureWrap::CLAMP_TO_BORDER:
        return GL_CLAMP_TO_BORDER;
    default:
        return GL_REPEAT;
    }
}

unsigned int TextureParams::getGLFilter(TextureFilter filter) {
    switch (filter) {
    case TextureFilter::NEAREST:
        return GL_NEAREST;
    case TextureFilter::LINEAR:
        return GL_LINEAR;
    default:
        return GL_LINEAR;
    }
}

void TextureParams::enable() const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLWrap(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLWrap(wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLFilter(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLFilter(magFilter));
}

Texture::Texture() {}

Texture::~Texture() {}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}

int Texture::getChannelCount() const {
    return channels;
}

Texture* Texture::create(Image* img) {
    return create(img, TextureParams::LINEAR);
}

Texture* Texture::create(Image* img, TextureParams params) {
    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    params.enable();

    unsigned int format = GL_RGBA;

    if (img->getChannelCount() == 3)
        format = GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, img->getWidth(), img->getHeight(), 0, format, GL_UNSIGNED_BYTE, img->getData());

    auto tex = new Texture();

    tex->texture  = texture;
    tex->width    = img->getWidth();
    tex->height   = img->getHeight();
    tex->channels = img->getChannelCount();
    tex->path = "";

    return tex;
}

Texture* Texture::create(unsigned int texture, unsigned int width, unsigned int height, unsigned int channelCount) {
    auto tex = new Texture();

    tex->texture  = texture;
    tex->width    = width;
    tex->height   = height;
    tex->channels = channelCount;
    tex->path = "";

    return tex;
}

Texture* Texture::load(const char* path) {
    return Texture::load(path, TextureParams::LINEAR);
}

Texture* Texture::load(const char* path, TextureParams params) {
    for (auto tex : loadedTextures) {
        if (tex->path == "")
            continue;

        if (tex->path == path)
            return tex;
    }

    auto img = Image::load(path);

    if (img == nullptr)
        return nullptr;

    auto tex = create(img, params);
    tex->path = path;

    loadedTextures.push_back(tex);

    delete img;
    return tex;
}

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::init() {
    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    TextureParams::NEAREST.enable();

    unsigned char data[16] = {
        255, 0, 255, 0,
        0,   0, 0,   0,
        0,   0, 0,   0,
        255, 0, 255, 0,
    };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    auto tex = new Texture();

    tex->texture  = texture;
    tex->width    = 1;
    tex->height   = 1;
    tex->channels = 4;

    EMPTY = tex;
    unbindAll();
}

void Texture::unbind(unsigned int unit) {
    EMPTY->bind(unit);
}

void Texture::unbindAll() {
    for (int i = 0; i < 20; i++)
        unbind(i);
}