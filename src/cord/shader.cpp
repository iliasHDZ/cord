#include "shader.hpp"

#include <glad/glad.h>

#include <fstream>

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

ShaderType shaderTypeFromFilename(std::string filename) {
    if (filename.ends_with(".vsh"))
        return ShaderType::Vertex;
    else if (filename.ends_with(".fsh"))
        return ShaderType::Fragment;
    else if (filename.ends_with(".vert"))
        return ShaderType::Vertex;
    else if (filename.ends_with(".frag"))
        return ShaderType::Fragment;

    return ShaderType::Vertex;
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

Shader* Shader::createFromFiles(std::vector<std::string> paths) {
    ShaderSource* sources = new ShaderSource[paths.size()];
    
    int i = 0;
    for (auto &p : paths) {
        std::ifstream f(p, std::ios::in | std::ios::binary);

        if (f.fail()) {
            error = "Shader: Failed to open file: " + p;
            return nullptr;
        }

        f.seekg(0, std::ios::end);
        size_t size = f.tellg();

        char* buffer = new char[size + 1];

        f.seekg(0);
        f.read(buffer, size);

        buffer[size] = 0;

        sources[i++] = {buffer, shaderTypeFromFilename(p)};
    }

    auto shader = create(std::vector<ShaderSource>(sources, sources + paths.size()));

    for (i = 0; i < paths.size(); i++)
        delete sources[i].src;

    delete sources;

    if (shader == nullptr)
        return nullptr;

    return shader;
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::setInt(const char* name, int value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setFloat(const char* name, float value) {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setBool(const char* name, bool value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setVec2(const char* name, glm::vec2 value) {
    glUniform2f(glGetUniformLocation(program, name), value.x, value.y);
}

void Shader::setVec3(const char* name, glm::vec3 value) {
    glUniform3f(glGetUniformLocation(program, name), value.x, value.y, value.z);
}

void Shader::setVec4(const char* name, glm::vec4 value) {
    glUniform4f(glGetUniformLocation(program, name), value.x, value.y, value.z, value.w);
}

void Shader::setMat3(const char* name, glm::mat3 value) {
    glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const char* name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setColor(const char* name, Color value) {
    glUniform4f(glGetUniformLocation(program, name), value.r, value.g, value.b, value.a);
}

void Shader::setColor3(const char* name, Color value) {
    glUniform3f(glGetUniformLocation(program, name), value.r, value.g, value.b);
}

void Shader::setTexture(const char* name, Texture* value, unsigned int unit) {
    if (value == nullptr)
        return;
    
    value->bind(unit);
    setInt(name, unit);
}

std::string Shader::getError() {
    return error;
}