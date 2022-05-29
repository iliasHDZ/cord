#pragma once

#include "camera3d.hpp"

#include <vector>

#define CORD_LIGHT_NONE 0
#define CORD_LIGHT_DIRECTIONAL 1
#define CORD_LIGHT_POINT 2
#define CORD_LIGHT_SPOT 3

#define CORD_LIGHT_COUNT 10

namespace cord {
    struct View3D_light {
        int  type;

        glm::vec3 position;
        glm::vec3 direction;

        Color ambient;
        Color diffuse;
        Color specular;

        float cutOff;
        float outerCutOff;
        Texture* spotMap;

        float constant;
        float linear;
        float quadratic;

        void apply(Shader* shader, int index);

        static void clearAll(Shader* shader);
    };

    class View3D : public Node {
    public:
        View3D(Camera3D* camera, unsigned int width, unsigned int height);

        virtual void init(Renderer* renderer);

        virtual void visitPreRender(Renderer* renderer);

        virtual void visitRender(Renderer* renderer);

        virtual void setViewMatrix(glm::mat4 viewMatrix);

        void addLight(View3D_light light);

        void applyLights(Shader* shader);

    private:
        virtual void preProcess(Renderer* renderer);

        virtual void postProcess(Renderer* renderer);

    public:
        static View3D* getCurrent();

    public:
        Camera3D* camera;

    private:
        static View3D* current;

        static bool renderCallbackSet;

        bool initialized;

        unsigned int width, height;

        glm::mat4 view;
        bool viewMatrixSet;

        std::vector<View3D_light> lights;

        Framebuffer* framebuffer;

        Texture* colorBuffer;
        Texture* depthBuffer;
    };
};