#pragma once

#include "vertex_buffer.hpp"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cord {

enum DataType {
    Float
};

unsigned int getGLDataType(DataType type);

class VertexArray {
public:
    VertexArray();

    static VertexArray* create();

    static VertexArray* from3DVertices(const float* vertices, unsigned int size);

    static VertexArray* from3DVertices(const std::vector<float> vertices);

    static VertexArray* from3DVertices(const std::vector<glm::vec3> vertices);

    VertexArray* addTexCoords(const float* texCoords, unsigned int size);

    VertexArray* addTexCoords(const std::vector<float> vertices);

    VertexArray* addTexCoords(const std::vector<glm::vec2> vertices);

    VertexArray* addNormals(const float* normals, unsigned int size);

    VertexArray* addNormals(const std::vector<float> vertices);

    VertexArray* addNormals(const std::vector<glm::vec3> vertices);

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
    unsigned int cIdx;

    std::vector<unsigned int> attribs;
};

}