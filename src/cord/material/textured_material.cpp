#include "textured_material.hpp"

using namespace cord;

TexturedMaterial::TexturedMaterial(Renderer* renderer, Color color)
    : BasicMaterial(renderer, color), texture(nullptr), specularMap(nullptr) {}
TexturedMaterial::TexturedMaterial(Renderer* renderer, Color color, float ambient, float diffuse, float specular, float shininess)
    : BasicMaterial(renderer, color, ambient, diffuse, specular, shininess), texture(nullptr), specularMap(nullptr) {}
TexturedMaterial::TexturedMaterial(Renderer* renderer, Color ambient, Color diffuse, Color specular, float shininess)
    : BasicMaterial(renderer, ambient, diffuse, specular, shininess), texture(nullptr), specularMap(nullptr) {}

TexturedMaterial::TexturedMaterial(Renderer* renderer, Texture* texture)
    : BasicMaterial(renderer, Color::WHITE, 0.0f, 0.0f, 0.5f, 16.0f), texture(texture), specularMap(nullptr) {}
TexturedMaterial::TexturedMaterial(Renderer* renderer, Texture* texture, Color specular, float shininess)
    : BasicMaterial(renderer, Color::BLACK, Color::BLACK, specular, shininess), texture(texture), specularMap(nullptr) {}
TexturedMaterial::TexturedMaterial(Renderer* renderer, Texture* texture, Texture* specularMap, float shininess)
    : BasicMaterial(renderer, Color::BLACK, Color::BLACK, Color::BLACK, shininess), texture(texture), specularMap(specularMap) {}

Shader* TexturedMaterial::use() {
    Shader* shader = renderer->getShader("tex_shape3d");

    Texture::unbindAll();

    shader->use();
    shader->setColor3("material.ambient", ambient);
    shader->setColor3("material.diffuse", diffuse);
    shader->setColor3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);

    if (texture != nullptr)
        shader->setTexture("material.diffuseMap", texture, 0);
    if (specularMap != nullptr)
        shader->setTexture("material.specularMap", specularMap, 1);

    shader->setBool("material.hasDiffuseMap", texture != nullptr);
    shader->setBool("material.hasSpecularMap", specularMap != nullptr);
    
    return shader;
}