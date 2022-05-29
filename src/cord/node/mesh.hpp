#include "node3d.hpp"
#include "../vertex_array.hpp"
#include "../color.hpp"

#include "../material/material.hpp"

#include <string>

namespace cord {

class Mesh : public Node3D {
public:
    Mesh(VertexArray* va, unsigned int vertCount);
    Mesh(VertexArray* va, unsigned int vertCount, Material* material);

    virtual void render(Renderer* renderer);

    virtual void update(float delta);

    static Node3D* import(Renderer* r, std::string path);

    static std::string getError();

public:
    Material* material;

private:
    static std::string error;

private:
    VertexArray* va;
    unsigned int vertCount;
};

}