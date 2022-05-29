#pragma once

#include "renderer.hpp"
#include "node/node.hpp"

namespace cord {

typedef double (*GetTimeFunc)();

typedef void (*UpdateFunc)(float delta);

class Director {
public:
    Director(GetTimeFunc getTimeFunc);
    virtual ~Director();

    void render();

    void update(float delta);

    void visit();

    void setUpdateFunc(UpdateFunc updateFunc);

    void setRoot(Node* root);

    bool init(int width, int height);

    void resize(int width, int height);

    void setBackgroundColor(Color color);

    Color getBackgroundColor();

    Renderer* getRenderer() const;

    static Director* create(int width, int height, GetTimeFunc getTimeFunc);

    static std::string getError();

    static Director* getCurrent();

private:
    static std::string error;

    static Director* current;

    Renderer* renderer;

    unsigned int maxFPS;

    double lastTime;
    double frameTimeCounter;
    double secondCounter;

    UpdateFunc updateFunc;

    unsigned int frameCounter;

    GetTimeFunc getTimeFunc;

    Color backgroundColor;

    Node* root;
};

}