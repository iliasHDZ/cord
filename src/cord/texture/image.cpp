#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

using namespace cord;

Image::Image() {}

Image::~Image() {
    stbi_image_free(data);
}

int Image::getWidth() const {
    return width;
}
    
int Image::getHeight() const {
    return height;
}

int Image::getChannelCount() const {
    return channels;
}

const unsigned char* Image::getData() const {
    return data;
}

Image* Image::load(const char* path) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (!data)
        return nullptr;

    Image* image = new Image();

    image->data = data;
    image->width = width;
    image->height = height;
    image->channels = channels;
    return image;
}