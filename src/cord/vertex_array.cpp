#include "vertex_array.hpp"

#include <glad/glad.h>

using namespace cord;

unsigned int cord::getGLDataType(DataType type) {
    switch (type) {
    case DataType::Float:
        return GL_FLOAT;
    }

    return 0;
}

VertexArray::VertexArray()
    : cIdx(0) {}

VertexArray* VertexArray::create() {
    auto va = new VertexArray();

    glGenVertexArrays(1, &va->vao);

    return va;
}

VertexArray* VertexArray::from3DVertices(const float* vertices, unsigned int size) {
    auto va = VertexArray::create();

    VertexBuffer vb((char*)vertices, size);

    va->attrib(&vb, 0, 3, DataType::Float, 3 * sizeof(float), 0);
    return va;
}

VertexArray* VertexArray::from3DVertices(const std::vector<float> vertices) {
    return VertexArray::from3DVertices(vertices.data(), vertices.size() * sizeof(float));
}

VertexArray* VertexArray::from3DVertices(const std::vector<glm::vec3> vertices) {
    return VertexArray::from3DVertices((float*)vertices.data(), vertices.size() * sizeof(glm::vec3));
}

VertexArray* VertexArray::addTexCoords(const float* texCoords, unsigned int size) {
    VertexBuffer vb((char*)texCoords, size);

    attrib(&vb, 2, 2, DataType::Float, 2 * sizeof(float), 0);
    return this;
}

VertexArray* VertexArray::addTexCoords(const std::vector<float> vertices) {
    return VertexArray::addTexCoords((float*)vertices.data(), vertices.size() * sizeof(float));
}

VertexArray* VertexArray::addTexCoords(const std::vector<glm::vec2> vertices) {
    return VertexArray::addTexCoords((float*)vertices.data(), vertices.size() * sizeof(glm::vec2));
}

VertexArray* VertexArray::addNormals(const float* normals, unsigned int size) {
    VertexBuffer vb((char*)normals, size);

    attrib(&vb, 1, 3, DataType::Float, 3 * sizeof(float), 0);
    return this;
}

VertexArray* VertexArray::addNormals(const std::vector<float> vertices) {
    return VertexArray::addNormals((float*)vertices.data(), vertices.size() * sizeof(float));
}

VertexArray* VertexArray::addNormals(const std::vector<glm::vec3> vertices) {
    return VertexArray::addNormals((float*)vertices.data(), vertices.size() * sizeof(glm::vec3));
}

void VertexArray::attrib(
    VertexBuffer* vb,
    unsigned int index,
    unsigned int components,
    DataType type,
    unsigned int stride,
    unsigned int offset
) {
    glBindVertexArray(vao);

    vb->bind();

    glVertexAttribPointer(
        index,
        components,
        getGLDataType(type),
        GL_FALSE,
        stride,
        (void*)offset
    );

    glEnableVertexAttribArray(index);

    attribs.push_back(index);
}

void VertexArray::enable() {
    glBindVertexArray(vao);

    /*for (auto &a : attribs)
        glEnableVertexAttribArray(a);*/
}

void VertexArray::disable() {
    /*for (auto &a : attribs)
        glDisableVertexAttribArray(a);*/
}
