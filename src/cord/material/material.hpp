#pragma once

#include "../renderer.hpp"

namespace cord {

class Material {
public:
    Material(Renderer* renderer);
    virtual ~Material();

    virtual Shader* use() = 0;

protected:
    Renderer* renderer;
};

}