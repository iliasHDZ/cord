#include "flat_material.hpp"

using namespace cord;

FlatMaterial::FlatMaterial(Renderer* renderer, Color color)
    : Material(renderer), color(color) {}

FlatMaterial::~FlatMaterial() {}

Shader* FlatMaterial::use() {
    Shader* shader = renderer->getShader("shape3d");

    shader->use();
    shader->setColor3("material.ambient", color);
    shader->setBool("material.flatShaded", true);
    return shader;
}