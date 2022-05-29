#pragma once

#include "basic_material.hpp"
#include "../texture/texture.hpp"

namespace cord {

class TexturedMaterial : public BasicMaterial {
public:
    TexturedMaterial(Renderer* renderer, Color color);
    TexturedMaterial(Renderer* renderer, Color color, float ambient, float diffuse, float specular, float shininess);
    TexturedMaterial(Renderer* renderer, Color ambient, Color diffuse, Color specular, float shininess);

    TexturedMaterial(Renderer* renderer, Texture* texture);
    TexturedMaterial(Renderer* renderer, Texture* texture, Color specular, float shininess);
    TexturedMaterial(Renderer* renderer, Texture* texture, Texture* specularMap, float shininess);

    virtual Shader* use();

public:
    Texture* texture;

    Texture* specularMap;
};

}