#include "basic_material.hpp"

#include <iostream>

using namespace cord;

BasicMaterial::BasicMaterial(Renderer* renderer, Color color)
    : Material(renderer), ambient(color), diffuse(Color::BLACK), specular(Color::BLACK), shininess(1.0f) {}

BasicMaterial::BasicMaterial(Renderer* renderer, Color color, float ambient, float diffuse, float specular, float shininess)
    : Material(renderer), ambient(color * ambient), diffuse(color * diffuse), specular(color * specular), shininess(shininess) {}

BasicMaterial::BasicMaterial(Renderer* renderer, Color ambient, Color diffuse, Color specular, float shininess)
    : Material(renderer), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

Shader* BasicMaterial::use() {
    Shader* shader = renderer->getShader("shape3d");

    shader->use();
    shader->setColor3("material.ambient", ambient);
    shader->setColor3("material.diffuse", diffuse);
    shader->setColor3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
    
    return shader;
}