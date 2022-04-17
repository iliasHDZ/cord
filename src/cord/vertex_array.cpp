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

VertexArray::VertexArray() {}

VertexArray* VertexArray::create() {
    auto va = new VertexArray();

    glGenVertexArrays(1, &va->vao);

    return va;
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
