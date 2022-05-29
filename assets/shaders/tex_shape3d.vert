#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform vec3 uViewPos;

out vec3 fragPos;
out vec3 normal;

out vec3 viewPos;

out vec2 texCoord;

void main() {
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));

    normal = mat3(transpose(inverse(model))) * aNor;
    viewPos = uViewPos;
    texCoord = aTexCoord;
}
