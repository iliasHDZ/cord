#pragma once

namespace cord {

class VertexBuffer {
public:
    VertexBuffer(char* data, int size);

    void bind();

private:
    unsigned int vbo;
};

}