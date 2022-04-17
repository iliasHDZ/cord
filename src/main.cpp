#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cord/renderer.hpp"

#include <fstream>

using namespace cord;

GLFWwindow* window;

int initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Cord Engine Testing", NULL, NULL);
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;
}

const char* readFileToString(std::string path) {
    std::ifstream f(path, std::ios::in | std::ios::binary);

    if (f.fail())
        return "";

    f.seekg(0, std::ios::end);
    size_t size = f.tellg();

    char* buffer = new char[size + 1];

    f.seekg(0);
    f.read(buffer, size);

    buffer[size] = 0;
    return buffer;
}

int main() {
    int ret;
    if ((ret = initWindow()) != 0)
        return ret;

    auto r = Renderer::create();

    r->resize(800, 600);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    auto va = VertexArray::create();

    VertexBuffer vb((char*)vertices, sizeof(vertices));

    va->attrib(
        &vb,
        0,
        3,
        DataType::Float,
        3 * sizeof(float),
        0
    );
    
    std::vector<ShaderSource> sources;

    auto vsh = readFileToString("./assets/shaders/basic.vsh");
    auto fsh = readFileToString("./assets/shaders/basic.fsh");

    sources.push_back({vsh, ShaderType::Vertex});
    sources.push_back({fsh, ShaderType::Fragment});

    delete vsh;
    delete fsh;

    auto s = Shader::create(sources);

    if (s == 0) {
        std::cout << Shader::getError() << std::endl;
        return -1;
    }

    while(!glfwWindowShouldClose(window)) {
        r->render();

        /*s->use();

        va->enable();
        glDrawArrays(GL_TRIANGLES, 0, 3);*/

        r->renderObject(s, va, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}