#pragma once

namespace cord {

class Image {
public:
    ~Image();

    int getWidth() const;
    
    int getHeight() const;

    int getChannelCount() const;

    const unsigned char* getData() const;
    
    static Image* load(const char* path);

private:
    Image();

private:
    unsigned char* data;
    int width;
    int height;
    int channels;
};

}