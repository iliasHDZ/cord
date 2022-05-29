#version 330 core
out vec4 FragColor;

uniform sampler2D colorBuffer;
uniform sampler2D depthBuffer;

uniform float near;
uniform float far;

uniform vec2 resolution;

in vec2 texCoord;

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 getColor(vec2 texCoord) {
    return texture(colorBuffer, texCoord).rgb;
}

vec3 getColorByPixel(vec2 pos) {
    return texture(colorBuffer, pos / resolution).rgb;
}

float getDepthByPixel(vec2 pos) {
    return texture(depthBuffer, pos / resolution).r;
}

vec3 blur(vec2 pos, float radius, float depth) {
    int kernelSize = int(radius * 2.0 + 1.0);

    float kernelSum;

    for (int i = 0; i < kernelSize; i++)
        for (int j = 0; j < kernelSize; j++) {
            vec2 pix = vec2(i - radius, j - radius);
            kernelSum += length(vec2(i - radius, j - radius));
        }

    vec3 color = vec3(0.0);

    for (int i = 0; i < kernelSize; i++)
        for (int j = 0; j < kernelSize; j++) {
            vec2 pix = vec2(i - radius, j - radius);
            color += getColorByPixel(pos + pix) * (length(pix) / kernelSum);
        }

    return color;
}

void main() {
    float depth = linearizeDepth(texture(depthBuffer, texCoord).r);

    float focal = min(linearizeDepth(texture(depthBuffer, vec2(0.5, 0.5)).r), 50.0);
    float blurAmount = min(abs(depth - focal), 5.0) / 2.0;

    vec3 color = blur(texCoord * resolution, blurAmount, depth);

    FragColor = vec4(color, 1.0);
}