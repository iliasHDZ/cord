#include "color.hpp"

using namespace cord;

#define CORD_COLORS_STATIC_DEF(N, R, G, B) \
    Color Color::N = {R, G, B};

CORD_COLORS_DEF(CORD_COLORS_STATIC_DEF)

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b), a(1.0f) {}

Color::Color(float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a) {}

Color Color::operator*(float a) {
    return {r * a, g * a, b * a, a};
}

Color Color::fromRGB(unsigned char r, unsigned char g, unsigned char b) {
    return Color(r / 255.0f, g / 255.0f, b / 255.0f);
}

Color Color::fromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
