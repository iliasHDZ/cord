#include "mesh.hpp"

#include "../material/basic_material.hpp"
#include "../material/textured_material.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

using namespace cord;

std::string Mesh::error = "";

Mesh::Mesh(VertexArray* va, unsigned int vertCount)
    : va(va), vertCount(vertCount), material(nullptr) {}

Mesh::Mesh(VertexArray* va, unsigned int vertCount, Material* material)
    : va(va), vertCount(vertCount), material(material) {}

void Mesh::render(Renderer* renderer) {
    if (va == nullptr)
        return;

    Shader* sh;

    if (material != nullptr)
        sh = material->use();
    else {
        BasicMaterial defaultMaterial(renderer, Color::WHITE);

        sh = defaultMaterial.use();
    }

    renderer->renderObject3D(sh, va, vertCount);
}

void Mesh::update(float delta) {
    //rotation.y += (delta * 90) * 0.5f;
}

static Renderer* renderer;
static std::string directory;
static std::string procError;

static Texture* processTexture(aiMaterial* mat, aiTextureType type) {
    aiString path;

    if (mat->GetTexture(type, 0, &path) == AI_SUCCESS) {
        Texture* tex = Texture::load((directory + "/" + path.C_Str()).c_str());

        if (tex == nullptr) {
            procError = std::string("Mesh: Failed to load texture: ") + path.C_Str();
            return nullptr;
        }

        return tex;
    }

    return nullptr;
}

static Material* processMaterial(aiMaterial* mat, const aiScene* scene, const std::string& path) {
    TexturedMaterial* tmat = new TexturedMaterial(renderer, Color::WHITE);

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        if ((tmat->texture = processTexture(mat, aiTextureType_DIFFUSE)) == nullptr) {
            delete tmat;
            return nullptr;
        }

    if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0)
        if ((tmat->specularMap = processTexture(mat, aiTextureType_SPECULAR)) == nullptr) {
            delete tmat;
            return nullptr;
        }

    tmat->shininess = 32.0f;

    return tmat;
}

static Mesh* processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace f = mesh->mFaces[i];
        for (unsigned int j = 0; j < f.mNumIndices; j++) {
            glm::vec3 pos, norm, tex;
            unsigned int idx = f.mIndices[j];

            pos.x = mesh->mVertices[idx].x;
            pos.y = mesh->mVertices[idx].y;
            pos.z = mesh->mVertices[idx].z;

            norm.x = mesh->mNormals[idx].x;
            norm.y = mesh->mNormals[idx].y;
            norm.z = mesh->mNormals[idx].z;

            if (mesh->mTextureCoords[0]) {
                tex.x = mesh->mTextureCoords[0][idx].x;
                tex.y = mesh->mTextureCoords[0][idx].y;
            } else {
                tex.x = 0.0f;
                tex.y = 0.0f;
            }

            positions.push_back(pos);
            normals.push_back(norm);
            texCoords.push_back(tex);
        }
    }

    Material* material;

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        material = processMaterial(mat, scene, directory);

        if (material == nullptr)
            return nullptr;
    } else
        material = new BasicMaterial(renderer, Color::WHITE);

    VertexArray* va = VertexArray::from3DVertices(positions)
        ->addNormals(normals)
        ->addTexCoords(texCoords);

    return new Mesh(va, positions.size(), material);
}

static bool processNode(Node3D* node, aiNode* ainode, const aiScene* scene) {
    for (unsigned int i = 0; i < ainode->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[ainode->mMeshes[i]];

        Mesh* m = processMesh(mesh, scene);
        if (m == nullptr) {
            for (int j = 0; j < node->count(); j++)
                delete node->get(j);
            return false;
        }

        node->add(m);
    }

    for (unsigned int i = 0; i < ainode->mNumChildren; i++) {
        if (!processNode(node, ainode->mChildren[i], scene))
            return false;
    }

    return true;
}

Node3D* Mesh::import(Renderer* r, std::string path) {
    renderer = r;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);// | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        error = std::string("Mesh: Error importing(Assimp): ") + importer.GetErrorString();
        return nullptr;
    }

    directory = path.substr(0, path.find_last_of('/'));

    Node3D* root = new Node3D();
    if (!processNode(root, scene->mRootNode, scene)) {
        delete root;
        error = procError;
        return nullptr;
    }

    return root;
}

std::string Mesh::getError() {
    return Mesh::error;
}