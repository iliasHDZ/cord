#include "view3d.hpp"

#include <string>
#include <iostream>

#include <glad/glad.h>

#include "../director.hpp"

using namespace cord;

View3D* View3D::current = nullptr;
bool View3D::renderCallbackSet = false;

void View3D_light::apply(Shader* shader, int index) {
    std::string array_name = "lights[" + std::to_string(index) + "]";

    shader->setInt((array_name + ".type").c_str(), type);

    shader->setVec3((array_name + ".position").c_str(), position);
    shader->setVec3((array_name + ".direction").c_str(), direction);

    shader->setColor3((array_name + ".ambient").c_str(), ambient);
    shader->setColor3((array_name + ".diffuse").c_str(), diffuse);
    shader->setColor3((array_name + ".specular").c_str(), specular);

    shader->setFloat((array_name + ".cutOff").c_str(), cutOff);
    shader->setFloat((array_name + ".outerCutOff").c_str(), outerCutOff);
    shader->setTexture("spotMap", spotMap, 5);
    shader->setBool((array_name + ".hasSpotMap").c_str(), spotMap != nullptr);

    shader->setFloat((array_name + ".constant").c_str(), constant);
    shader->setFloat((array_name + ".linear").c_str(), linear);
    shader->setFloat((array_name + ".quadratic").c_str(), quadratic);
}

void View3D_light::clearAll(Shader* shader) {
    for (int i = 0; i < CORD_LIGHT_COUNT; i++)
        shader->setInt(("lights[" + std::to_string(i) + "].type").c_str(), CORD_LIGHT_NONE);
}

View3D::View3D(Camera3D* camera, unsigned int width, unsigned int height)
    : camera(camera), width(width), height(height), initialized(false) {}

bool done = false;

static void renderCallback(Renderer* renderer, Shader* shader) {
    View3D* view = View3D::getCurrent();
    if (view == nullptr)
        return;

    shader->setFloat("near", view->camera->near);
    shader->setFloat("far",  view->camera->far);

    View3D_light::clearAll(shader);
    view->applyLights(shader);
}

void View3D::init(Renderer* renderer) {
    framebuffer = renderer->createFramebuffer();

    colorBuffer = framebuffer->attachColor();
    depthBuffer = framebuffer->attachDepth();
}

void View3D::visitPreRender(Renderer* renderer) {
    if (!renderCallbackSet) {
        renderer->onRender3D(renderCallback);
        renderCallbackSet = true;
    }

    if (!initialized) {
        init(renderer);
        initialized = true;
    }

    if (camera == nullptr)
        return;

    if (current == nullptr) {
        viewMatrixSet = false;
        lights.clear();

        current = this;
        Node::visitPreRender(renderer);
        current = nullptr;
    } else
        Node::visitPreRender(renderer);
}

void View3D::visitRender(Renderer* renderer) {
    if (camera == nullptr || !viewMatrixSet)
        return;

    if (current == nullptr) {
        current = this;

        preProcess(renderer);

        Node::visitRender(renderer);

        postProcess(renderer);

        current = nullptr;
    } else
        Node::visitRender(renderer);
}

void View3D::setViewMatrix(glm::mat4 viewMatrix) {
    viewMatrixSet = true;
    view = viewMatrix;
}

void View3D::addLight(View3D_light light) {
    lights.push_back(light);
}

void View3D::applyLights(Shader* shader) {
    for (int i = 0; i < glm::min((int)lights.size(), CORD_LIGHT_COUNT); i++)
        lights[i].apply(shader, i);
}

void View3D::preProcess(Renderer* renderer) {
    renderer->setProjection3D(camera->getProjectionMatrix(width, height));
    renderer->setView3D(view);

    renderer->setViewPos(camera->position);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    framebuffer->bind();

    auto dir = Director::getCurrent();

    renderer->clear(dir->getBackgroundColor());
}

void View3D::postProcess(Renderer* renderer) {
    framebuffer->unbind();

    glDisable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);

    Shader* finalsh = renderer->getShader("final");

    finalsh->use();

    finalsh->setTexture("colorBuffer", colorBuffer, 0);
    finalsh->setTexture("depthBuffer", depthBuffer, 1);

    finalsh->setFloat("near", camera->near);
    finalsh->setFloat("far", camera->far);

    finalsh->setVec2("resolution", {renderer->getWidth(), renderer->getHeight()});

    renderer->drawScreenQuad(finalsh);
}

View3D* View3D::getCurrent() {
    return current;
}