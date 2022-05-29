#include "director.hpp"

#include <iostream>
#include <glad/glad.h>

#include "texture/texture.hpp"

using namespace cord;

std::string Director::error = "";
Director* Director::current;

Director::Director(GetTimeFunc getTimeFunc)
    : getTimeFunc(getTimeFunc), maxFPS(60), renderer(nullptr), root(nullptr), backgroundColor(0.2f, 0.3f, 0.3f) {}

Director::~Director() {
    delete renderer;
    delete root;
}

void Director::render() {
    renderer->clear(backgroundColor);

    if (root != nullptr) {
        root->visitPreRender(renderer);
        root->visitRender(renderer);
    }
}

void Director::update(float delta) {
    if (root != nullptr)
        root->visitUpdate(delta);

    if (updateFunc != nullptr)
        updateFunc(delta);
}

void Director::visit() {
    double now = getTimeFunc();
    double delta = now - lastTime;
    lastTime = now;

    const double frameTime = 1.0 / maxFPS;
    //frameTimeCounter += delta;
    secondCounter += delta;

    update(delta);

    //std::cout << getTimeFunc() << std::endl;

    /*if (frameTimeCounter >= frameTime * 10) {
        render();
        frameTimeCounter = 0;
        frameCounter++;

        //std::cout << "LONG FRAME!" << std::endl;
    }
    
    while (frameTimeCounter >= frameTime) {
        render();
        frameTimeCounter -= frameTime;
        frameCounter++;

        //std::cout << "FRAME!" << std::endl;
    }*/
    render();
    frameCounter++;

    while (secondCounter > 1.0) {
        std::cout << "FPS: " << frameCounter << std::endl;
        secondCounter -= 1.0;
        frameCounter = 0;
    }
}

void Director::setUpdateFunc(UpdateFunc updateFunc) {
    this->updateFunc = updateFunc;
}

void Director::setRoot(Node* root) {
    this->root = root;
}

bool Director::init(int width, int height) {
    renderer = Renderer::create();
    
    if (renderer == nullptr) {
        error = Renderer::getError();
        return false;
    }

    current = this;

    lastTime = getTimeFunc();
    frameTimeCounter = 0;

    updateFunc = nullptr;

    Texture::init();

    renderer->resize(width, height);
    return true;
}

void Director::resize(int width, int height) {
    renderer->resize(width, height);
}

void Director::setBackgroundColor(Color color) {
    backgroundColor = color;
}

Color Director::getBackgroundColor() {
    return backgroundColor;
}

Renderer* Director::getRenderer() const {
    return renderer;
}

Director* Director::create(int width, int height, GetTimeFunc getTimeFunc) {
    auto d = new Director(getTimeFunc);

    if (!d->init(width, height)) {
        delete d;
        return nullptr;
    }

    return d;
}

std::string Director::getError() {
    return error;
}

Director* Director::getCurrent() {
    return current;
}