#pragma once

#include "node3d.hpp"

namespace cord {

class PointLight : public Node3D {
public:
    PointLight(float linear, float quadratic, Color color);
    PointLight(float linear, float quadratic, Color color, float intensity);
    PointLight(float linear, float quadratic, Color ambient, Color diffuse, Color specular);
    PointLight(float linear, float quadratic, Color ambient, Color diffuse, Color specular, float intensity);

    virtual void preRender(Renderer* renderer);

public:
    float linear, quadratic;
    float intensity;
    
    Color ambient;
    Color diffuse;
    Color specular;
};

}