#include "vertex_buffer.hpp"

#include <glad/glad.h>

using namespace cord;

VertexBuffer::VertexBuffer(char* data, int size) {
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}