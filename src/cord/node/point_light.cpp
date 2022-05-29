#include "point_light.hpp"
#include "view3d.hpp"

using namespace cord;

PointLight::PointLight(float linear, float quadratic, Color color)
    : linear(linear), quadratic(quadratic), ambient(color * 0.2f), diffuse(color * 0.4f), specular(color * 0.4f), intensity(1.0) {}
PointLight::PointLight(float linear, float quadratic, Color color, float intensity)
    : linear(linear), quadratic(quadratic), ambient(color * 0.2f), diffuse(color * 0.4f), specular(color * 0.4f), intensity(intensity) {}
PointLight::PointLight(float linear, float quadratic, Color ambient, Color diffuse, Color specular)
    : linear(linear), quadratic(quadratic), ambient(ambient), diffuse(diffuse), specular(specular), intensity(1.0) {}
PointLight::PointLight(float linear, float quadratic, Color ambient, Color diffuse, Color specular, float intensity)
    : linear(linear), quadratic(quadratic), ambient(ambient), diffuse(diffuse), specular(specular), intensity(intensity) {}

void PointLight::preRender(Renderer* renderer) {
    View3D* view = View3D::getCurrent();
    if (view == nullptr)
        return;

    View3D_light light = {
        .type = CORD_LIGHT_POINT,

        .position = absolutePosition,

        .ambient = ambient * intensity,
        .diffuse = diffuse * intensity,
        .specular = specular * intensity,

        .constant = 1.0f,
        .linear = linear,
        .quadratic = quadratic
    };

    view->addLight(light);
}