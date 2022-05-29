#pragma once

#include "texture/texture.hpp"
#include "renderer.hpp"

#include <vector>

namespace cord {

class Renderer;

class Framebuffer {
public:
    Framebuffer(Renderer* renderer);
    virtual ~Framebuffer();

    void bind();
    static void unbind();

    Texture* attachColor();

    Texture* attachDepth();

private:
    void init();

private:
    Renderer* renderer;

    unsigned int framebuffer;

    std::vector<Texture*> textures;
};

}