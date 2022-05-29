#pragma once

#include "node.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace cord {
    class Node3D : public Node {
    public:
        Node3D();

        virtual void visitPreRender(Renderer* renderer);

        virtual void visitRender(Renderer* renderer);
        
        virtual void visitUpdate(float delta);

        virtual NodeTopType getTopType();

        glm::mat4 getModelMatrix();

    public:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;

        glm::vec3 absolutePosition;
        glm::vec3 absoluteRotation;

        glm::mat4 absoluteModel;

    private:
        static std::vector<glm::vec3> rotation_stack;
    };
};