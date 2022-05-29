#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "color.hpp"
#include "texture/texture.hpp"

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

ShaderType shaderTypeFromFilename(std::string filename);

class Shader {
public:
    Shader();

    virtual bool init(std::vector<ShaderSource> sources);

    static Shader* create(std::vector<ShaderSource> sources);

    static Shader* createFromFiles(std::vector<std::string> paths);

    void use();

    void setInt(const char* name, int value);

    void setFloat(const char* name, float value);

    void setBool(const char* name, bool value);

    void setVec2(const char* name, glm::vec2 value);

    void setVec3(const char* name, glm::vec3 value);

    void setVec4(const char* name, glm::vec4 value);

    void setMat3(const char* name, glm::mat3 value);

    void setMat4(const char* name, glm::mat4 value);

    void setColor(const char* name, Color value);

    void setColor3(const char* name, Color value);

    void setTexture(const char* name, Texture* value, unsigned int unit);

    static std::string getError();

private:
    static std::string error;

    unsigned int program;
};

}