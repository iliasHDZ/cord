#pragma once

#include "vertex_buffer.hpp"

#include <vector>

namespace cord {

enum DataType {
    Float
};

unsigned int getGLDataType(DataType type);

class VertexArray {
public:
    VertexArray();

    static VertexArray* create();

    void attrib(
        VertexBuffer* vb,
        unsigned int index,
        unsigned int components,
        DataType type,
        unsigned int stride,
        unsigned int offset
    );

    void enable();
    void disable();

private:
    unsigned int vao;

    std::vector<unsigned int> attribs;
};

}