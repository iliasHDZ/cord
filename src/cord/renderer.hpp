#pragma once

#include "shader.hpp"
#include "vertex_array.hpp"

namespace cord {

class Renderer {
public:
    Renderer();

    virtual void render();

    virtual void renderObject(Shader* s, VertexArray* va, unsigned int count);

    virtual void init();

    void resize(int width, int height);

    static Renderer* create();
};

}