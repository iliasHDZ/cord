#pragma once

#include "point_light.hpp"

namespace cord {

class SpotLight : public PointLight {
public:
    SpotLight(float linear, float quadratic, Color color, float cutOff, float outerCutOff);
    SpotLight(float linear, float quadratic, Color ambient, Color diffuse, Color specular, float cutOff, float outerCutOff);

    virtual void preRender(Renderer* renderer);

public:
    float cutOff;
    float outerCutOff;
};

}