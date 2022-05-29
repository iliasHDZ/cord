#include "renderer.hpp"

#include <glad/glad.h>

#include <iostream>

using namespace cord;

std::string Renderer::error = "";

VertexArray* Renderer::quad = nullptr;
VertexArray* Renderer::large_quad = nullptr;

const float Renderer::quad_vertices[] = {
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
};

const float Renderer::large_quad_vertices[] = {
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
};

Shader* Renderer::shape2d_shader = nullptr;
Shader* Renderer::shape3d_shader = nullptr;
Shader* Renderer::tex_shape3d_shader = nullptr;
Shader* Renderer::final_shader = nullptr;

Renderer::Renderer() {}

void Renderer::renderObject(Shader* s, VertexArray* va, unsigned int count) {
    s->use();

    va->enable();

    glDrawArrays(GL_TRIANGLES, 0, count);

    va->disable();
}

int ist = 0;

void Renderer::renderObject3D(Shader* s, VertexArray* va, unsigned int count) {
    s->use();

    emitRender3D(s);

    s->setMat4("model", currentTransform3D());
    s->setMat4("view", view3d);
    s->setMat4("proj", proj3d);

    s->setVec3("uViewPos", viewPos);

    renderObject(s, va, count);
}

void Renderer::drawRectangle(glm::vec2 pos, glm::vec2 size, Color color) {
    shape2d_shader->use();
    shape2d_shader->setMat4("proj", proj2d);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos.x + size.x / 2, pos.y + size.y / 2, 0.0f));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

    shape2d_shader->setMat4("model", model);

    shape2d_shader->setColor("color", color);

    renderObject(shape2d_shader, quad, 6);
}

void Renderer::drawScreenQuad(Shader* shader) {
    renderObject(shader, large_quad, 6);
}

bool Renderer::init() {
    quad       = VertexArray::from3DVertices(quad_vertices, sizeof(quad_vertices));
    large_quad = VertexArray::from3DVertices(large_quad_vertices, sizeof(large_quad_vertices));

    shape2d_shader = loadShader({"assets/shaders/shape2d.vert", "assets/shaders/shape2d.frag"});
    if (shape2d_shader == nullptr)
        return false;

    shape3d_shader = loadShader({"assets/shaders/shape3d.vert", "assets/shaders/shape3d.frag"});
    if (shape3d_shader == nullptr)
        return false;

    tex_shape3d_shader = loadShader({"assets/shaders/tex_shape3d.vert", "assets/shaders/tex_shape3d.frag"});
    if (tex_shape3d_shader == nullptr)
        return false;

    final_shader = loadShader({"assets/shaders/final.vert", "assets/shaders/final.frag"});
    if (final_shader == nullptr)
        return false;

    return true;
}

void Renderer::resize(int width, int height) {
    proj2d = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
    glViewport(0, 0, width, height);

    this->width  = width;
    this->height = height;
}

unsigned int Renderer::getWidth() const {
    return width;
}

unsigned int Renderer::getHeight() const {
    return height;
}

Framebuffer* Renderer::createFramebuffer() {
    return new Framebuffer(this);
}

void Renderer::clear(Color color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::mat4 Renderer::currentTransform3D() {
    if (transform_stack.size() == 0)
        return glm::mat4(1.0f);

    return transform_stack.back();
}

void Renderer::pushTransform3D(glm::mat4 transform) {
    auto current = currentTransform3D();

    transform_stack.push_back(current * transform);
}

void Renderer::popTransform3D() {
    transform_stack.pop_back();
}

glm::vec3 Renderer::transformPoint3D(glm::vec3 point) {
    return currentTransform3D() * glm::vec4(point, 1.0f);
}

glm::vec3 Renderer::transformDirection3D(glm::vec3 direction) {
    return currentTransform3D() * glm::vec4(direction, 0.0f);
}

void Renderer::setProjection3D(glm::mat4 proj) {
    proj3d = proj;
}

void Renderer::setView3D(glm::mat4 view) {
    view3d = view;
}

void Renderer::setViewPos(glm::vec3 pos) {
    viewPos = pos;
}

Shader* Renderer::getShader(const std::string name) {
    if (name == "shape2d")
        return shape2d_shader;
    else if (name == "shape3d")
        return shape3d_shader;
    else if (name == "tex_shape3d")
        return tex_shape3d_shader;
    else if (name == "final")
        return final_shader;

    return nullptr;
}

void Renderer::onRender3D(Render3DCallback callback) {
    render3DCallbacks.push_back(callback);
}

Renderer* Renderer::create() {
    auto r = new Renderer();

    if (!r->init()) {
        delete r;
        return nullptr;
    }

    return r;
}

std::string Renderer::getError() {
    return error;
}

glm::vec3 Renderer::normalFromTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    auto a = p2 - p1;
    auto b = p3 - p1;

    return glm::normalize(glm::cross(a, b));
}

Shader* Renderer::loadShader(std::vector<std::string> paths) {
    Shader* s = Shader::createFromFiles(paths);

    if (s == nullptr) {
        error = "Failed to load shader: " + Shader::getError();
        return nullptr;
    }

    return s;
}

void Renderer::emitRender3D(Shader* shader) {
    for (auto& c : render3DCallbacks)
        c(this, shader);
}