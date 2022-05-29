#include "node3d.hpp"

#include <iostream>

using namespace cord;

Node3D::Node3D()
    : position(0), rotation(0), scale(1.0f) {}

std::vector<glm::vec3> Node3D::rotation_stack;

void Node3D::visitPreRender(Renderer* renderer) {
    renderer->pushTransform3D(getModelMatrix());

    absolutePosition = renderer->transformPoint3D(glm::vec3(0.0f));

    rotation_stack.push_back(rotation);

    absoluteRotation = glm::vec3(0.0f);
    for (auto rot : rotation_stack)
        absoluteRotation += rot;

    Node::visitPreRender(renderer);

    rotation_stack.pop_back();

    absoluteModel = renderer->currentTransform3D();
    renderer->popTransform3D();
}

void Node3D::visitRender(Renderer* renderer) {
    renderer->pushTransform3D(absoluteModel);

    render(renderer);

    renderer->popTransform3D();

    for (auto child : children)
        child->visitRender(renderer);
}

void Node3D::visitUpdate(float delta) {
    Node::visitUpdate(delta);
}

NodeTopType Node3D::getTopType() {
    return NODETT_NODE3D;
}

glm::mat4 Node3D::getModelMatrix() {
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    return model;
}