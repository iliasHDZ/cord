#include "spot_light.hpp"
#include "view3d.hpp"

using namespace cord;

SpotLight::SpotLight(float linear, float quadratic, Color color, float cutOff, float outerCutOff)
    : PointLight(linear, quadratic, color), cutOff(cutOff), outerCutOff(outerCutOff) {}
SpotLight::SpotLight(float linear, float quadratic, Color ambient, Color diffuse, Color specular, float cutOff, float outerCutOff)
    : PointLight(linear, quadratic, ambient, diffuse, specular), cutOff(cutOff), outerCutOff(outerCutOff) {}

void SpotLight::preRender(Renderer* renderer) {
    View3D* view = View3D::getCurrent();
    if (view == nullptr)
        return;

    View3D_light light = {
        .type = CORD_LIGHT_SPOT,

        .position = absolutePosition,
        .direction = renderer->transformDirection3D({0, 0, 1}),

        .ambient = ambient * intensity,
        .diffuse = diffuse * intensity,
        .specular = specular * intensity,

        .cutOff = glm::cos(glm::radians(cutOff)),
        .outerCutOff = glm::cos(glm::radians(outerCutOff)),

        .constant = 1.0f,
        .linear = linear,
        .quadratic = quadratic
    };

    view->addLight(light);
}