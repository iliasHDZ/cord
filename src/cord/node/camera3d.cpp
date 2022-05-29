#include "camera3d.hpp"
#include "view3d.hpp"

#include <iostream>

using namespace cord;

Camera3D::Camera3D(float fov, float near, float far)
    : fov(fov), near(near), far(far) {}

void Camera3D::preRender(Renderer* renderer) {
    View3D* view = View3D::getCurrent();
    if (view == nullptr)
        return;

    if (view->camera != this)
        return;

    view->setViewMatrix(getViewMatrix(renderer));
}

glm::mat4 Camera3D::getViewMatrix(Renderer* renderer) {
    glm::vec3 pos = renderer->transformPoint3D(glm::vec3(0.0f));
    direction = renderer->transformDirection3D(glm::vec3(0.0f, 0.0f, 1.0f));
    up = renderer->transformDirection3D(glm::vec3(0.0f, 1.0f, 0.0f));

    return glm::lookAt(pos, pos + direction, up);
}

glm::vec3 Camera3D::getDirection() {
    return glm::vec3(getModelMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera3D::getUp() {
    return glm::vec3(getModelMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
}

glm::mat4 Camera3D::getProjectionMatrix(float width, float height) {
    return glm::perspective(glm::radians(fov), width / height, near, far);
}