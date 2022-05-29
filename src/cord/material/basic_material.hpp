#pragma once

#include "material.hpp"

namespace cord {

class BasicMaterial : public Material {
public:
    BasicMaterial(Renderer* renderer, Color color);
    BasicMaterial(Renderer* renderer, Color color, float ambient, float diffuse, float specular, float shininess);
    BasicMaterial(Renderer* renderer, Color ambient, Color diffuse, Color specular, float shininess);

    virtual Shader* use();

public:
    Color ambient, diffuse, specular;
    float shininess;
};

}