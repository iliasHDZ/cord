#include "shader.hpp"

#include <glad/glad.h>

using namespace cord;

namespace cord {

const char* shaderTypeToString(ShaderType type) {
    switch (type) {
    case ShaderType::Vertex:
        return "Vertex";
    case ShaderType::Fragment:
        return "Fragment";
    }

    return "<unknown>";
}

unsigned int shaderTypeToGL(ShaderType type) {
    switch (type) {
    case ShaderType::Vertex:
        return GL_VERTEX_SHADER;
    case ShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
    }

    return 0;
}

std::string Shader::error = "";

}

Shader::Shader() {}

bool Shader::init(std::vector<ShaderSource> sources) {
    int success, length;

    program = glCreateProgram();

    std::vector<unsigned int> shaders;

    for (auto &s : sources) {
        unsigned int shader = glCreateShader(shaderTypeToGL(s.type));

        glShaderSource(shader, 1, &s.src, nullptr);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            char *message = new char[length];
            glGetShaderInfoLog(shader, length, &length, message);

            error = shaderTypeToString(s.type) + std::string(" shader error: ") + std::string(message);

            delete[] message;

            glDeleteShader(shader);
            return false;
        }

        glAttachShader(program, shader);

        shaders.push_back(shader);
    }

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        char *message = new char[length];
        glGetProgramInfoLog(program, 512, NULL, message);

        error = std::string("Program error: ") + std::string(message);

        delete[] message;

        glDeleteProgram(program);
    }

    for (auto &s : shaders)
        glDeleteShader(s);

    return success;
}

Shader* Shader::create(std::vector<ShaderSource> sources) {
    auto s = new Shader();

    if (!s->init(sources)) {
        delete s;
        return nullptr;
    }

    return s;
}

void Shader::use() {
    glUseProgram(program);
}

std::string Shader::getError() {
    return error;
}