#pragma once

#include "shader.hpp"
#include "vertex_array.hpp"

#include "framebuffer.hpp"

#include <vector>

namespace cord {

class Renderer;

class Framebuffer;

typedef void (*Render3DCallback)(Renderer* renderer, Shader* shader);

class Renderer {
public:
    Renderer();

    virtual void renderObject(Shader* s, VertexArray* va, unsigned int count);

    virtual void renderObject3D(Shader* s, VertexArray* va, unsigned int count);

    virtual void drawRectangle(glm::vec2 pos, glm::vec2 size, Color color);

    virtual void drawScreenQuad(Shader* shader);

    virtual bool init();

    void resize(int width, int height);

    unsigned int getWidth() const;

    unsigned int getHeight() const;

    Framebuffer* createFramebuffer();

    void clear(Color color);

    glm::mat4 currentTransform3D();

    void pushTransform3D(glm::mat4 transform);

    void popTransform3D();

    glm::vec3 transformPoint3D(glm::vec3 point);

    glm::vec3 transformDirection3D(glm::vec3 direction);

    void setProjection3D(glm::mat4 proj);

    void setView3D(glm::mat4 view);

    void setViewPos(glm::vec3 viewPos);

    Shader* getShader(const std::string name);

    void onRender3D(Render3DCallback callback);

public:
    static Renderer* create();

    static std::string getError();

    static glm::vec3 normalFromTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

private:
    Shader* loadShader(std::vector<std::string> paths);

    void emitRender3D(Shader* shader);

private:
    static std::string error;

    static VertexArray* quad;
    static VertexArray* large_quad;
    static const float quad_vertices[];
    static const float large_quad_vertices[];

    static Shader* shape2d_shader;
    static Shader* shape3d_shader;
    static Shader* tex_shape3d_shader;
    static Shader* final_shader;

    unsigned int width;
    unsigned int height;

    std::vector<Render3DCallback> render3DCallbacks;

    std::vector<glm::mat4> transform_stack;

    glm::mat4 proj2d;
    glm::mat4 proj3d;

    glm::mat4 view3d;

    glm::vec3 viewPos;
};

}
