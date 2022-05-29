#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CORD_COLORS_DEF(D) \
    D(BLACK,   0, 0, 0) \
    D(WHITE,   1, 1, 1) \
    D(RED,     1, 0, 0) \
    D(GREEN,   0, 1, 0) \
    D(BLUE,    0, 0, 1) \
    D(YELLOW,  1, 1, 0) \
    D(CYAN,    0, 1, 1) \
    D(MAGENTA, 1, 0, 1) \

#define CORD_COLORS_STATIC(N, R, G, B) \
    static Color N;

namespace cord {

class Color {
public:
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);

    Color operator*(float a);

    float r, g, b, a;

    static Color fromRGB(unsigned char r, unsigned char g, unsigned char b);
    static Color fromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    CORD_COLORS_DEF(CORD_COLORS_STATIC)
};

}