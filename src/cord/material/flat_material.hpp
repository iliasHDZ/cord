#pragma once

#include "material.hpp"

namespace cord {

class FlatMaterial : public Material {
public:
    FlatMaterial(Renderer* renderer, Color color);
    virtual ~FlatMaterial();

    virtual Shader* use();

protected:
    Color color;
};

}