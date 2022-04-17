#include "renderer.hpp"

#include <glad/glad.h>

using namespace cord;

Renderer::Renderer() {}

void Renderer::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::renderObject(Shader* s, VertexArray* va, unsigned int count) {
    s->use();

    va->enable();

    glDrawArrays(GL_TRIANGLES, 0, count);

    va->disable();
}

void Renderer::init() {
    
}

void Renderer::resize(int width, int height) {
    glViewport(0, 0, width, height);
}

Renderer* Renderer::create() {
    auto r = new Renderer();

    r->init();

    return r;
}