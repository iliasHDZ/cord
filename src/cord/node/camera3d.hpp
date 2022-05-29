#pragma once

#include "node3d.hpp"

namespace cord {
    class Camera3D : public Node3D {
    public:
        Camera3D(float fov, float near, float far);

        virtual void preRender(Renderer* renderer);

        glm::mat4 getViewMatrix(Renderer* renderer);

        glm::vec3 getDirection();

        glm::vec3 getUp();

        glm::mat4 getProjectionMatrix(float width, float height);

    public:
        float fov;

        float near;
        float far;

        glm::vec3 direction;
        glm::vec3 up;
    };
};