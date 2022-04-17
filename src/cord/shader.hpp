#pragma once

#include <string>
#include <vector>

namespace cord {

enum class ShaderType {
    Vertex,
    Fragment
};

struct ShaderSource {
    const char* src;
    ShaderType type;
};

const char* shaderTypeToString(ShaderType type);

unsigned int shaderTypeToGL(ShaderType type);

class Shader {
public:
    Shader();

    virtual bool init(std::vector<ShaderSource> sources);

    static Shader* create(std::vector<ShaderSource> sources);

    void use();

    static std::string getError();

private:
    static std::string error;

    unsigned int program;
};

}