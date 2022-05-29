#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cord/director.hpp"

#include "cord/node/mesh.hpp"
#include "cord/node/view3d.hpp"
#include "cord/node/spot_light.hpp"

#include "cord/material/textured_material.hpp"
#include "cord/material/flat_material.hpp"

#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace cord;

GLFWwindow* window;

int initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Cord Engine Testing", NULL, NULL);
    
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

Director* director;

void window_resize(GLFWwindow* window, int width, int height) {
    director->resize(width, height);
}

double getTime() {
    return glfwGetTime();
}

Camera3D* camera;

void updateFunc(float delta) {
    const float cameraSpeed = 4.0f * delta; // adjust accordingly

    glm::vec3 cameraFront = camera->getDirection();
    glm::vec3 cameraUp    = camera->getUp();

    glm::vec3 cameraPos = camera->position;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    camera->position = cameraPos;
}

double prevMX, prevMY;
bool mouseSet = false;

void mouseMove(GLFWwindow* window, double xpos, double ypos) {
    if (!mouseSet) {
        prevMX = xpos;
        prevMY = ypos;
        mouseSet = true;
    }

    float xoffset = xpos - prevMX;
    float yoffset = ypos - prevMY;

    prevMX = xpos;
    prevMY = ypos;

    camera->rotation.x += 0.1f * yoffset;
    camera->rotation.y -= 0.1f * xoffset;

    if (camera->rotation.x > 89.0f)
        camera->rotation.x = 89.0f;

    if (camera->rotation.x < -89.0f)
        camera->rotation.x = -89.0f;
}

int main() {
    int ret;
    if ((ret = initWindow()) != 0)
        return ret;

    director = Director::create(1280, 720, getTime);
    
    if (director == nullptr) {
        std::cout << Director::getError() << std::endl;
        return -1;
    }

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    float normals[] = {
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f, 
         0.0f,  0.0f, -1.0f, 
         0.0f,  0.0f, -1.0f, 
         0.0f,  0.0f, -1.0f, 
         0.0f,  0.0f, -1.0f, 

         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,

        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,

         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,

         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f
    };

    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    director->setBackgroundColor(Color::fromRGB(128, 169, 237));

    auto containerTex = Texture::load("assets/textures/container.png");
    auto containerSpecularTex = Texture::load("assets/textures/container_specular.png");

    if (!containerTex || !containerSpecularTex) {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }

    auto va = VertexArray::from3DVertices(vertices, sizeof(vertices))
        ->addNormals(normals, sizeof(normals))
        ->addTexCoords(texCoords, sizeof(texCoords));

    TexturedMaterial* mat1 = new TexturedMaterial(director->getRenderer(), containerTex, containerSpecularTex, 32.0f);
    TexturedMaterial* mat2 = new TexturedMaterial(director->getRenderer(), containerTex, containerSpecularTex, 32.0f);
    TexturedMaterial* mat3 = new TexturedMaterial(director->getRenderer(), containerTex, containerSpecularTex, 32.0f);

    FlatMaterial* mat4 = new FlatMaterial(director->getRenderer(), Color::WHITE);

    auto mesh = new Mesh(va, 12 * 3);
    camera = new Camera3D(90, 0.1f, 100.0f);

    camera->position = {0, 1, -3};
    camera->rotation = {0, 0, 0};

    auto flashlight = new SpotLight(0.09f, 0.032f, Color::fromRGB(242, 245, 211), 30.5f, 36.0f);

    flashlight->intensity = 2.0f;

    flashlight->ambient = {0.05f, 0.05f, 0.05f};

    flashlight->position.y = -0.1f;

    //camera->add(flashlight);

    auto view = new View3D(camera, 1280, 720);

    view->add(camera);
    view->add(mesh);

    auto mesh2 = new Mesh(va, 12 * 3);
    mesh->add(mesh2);

    mesh->rotation.z = 10;

    mesh->position = {10, 0, 0};

    mesh2->position = {0, 2, 0};

    std::srand(std::time(nullptr));

    /*for (int i = 0; i < 12; i++) {
        auto m = new Mesh(va, 12 * 3);

        m->position = {
            ((float)std::rand() / RAND_MAX - 0.5f) * 10,
            ((float)std::rand() / RAND_MAX - 0.5f) * 10,
            ((float)std::rand() / RAND_MAX - 0.5f) * 10
        };

        m->rotation = {
            ((float)std::rand() / RAND_MAX) * 360,
            ((float)std::rand() / RAND_MAX) * 360,
            ((float)std::rand() / RAND_MAX) * 360
        };

        m->material = mat1;

        view->add(m);
    }*/

    mesh->material  = mat1;
    mesh2->material = mat2;

    auto lightMesh = new Mesh(va, 12 * 3);
    lightMesh->material = mat4;
    lightMesh->position = {4.0f, 0, 0};
    lightMesh->scale = {0.2f, 0.2f, 0.2f};

    Color color = Color::fromRGB(194, 217, 255);

    auto light = new PointLight(0.007, 0.0002, color * 0.2f, color * 0.4f, color * 0.1f, 2.0f);

    lightMesh->add(light);
    lightMesh->position.y = 40.0f;

    view->add(lightMesh);

    auto object = Mesh::import(director->getRenderer(), "assets/models/sponza/sponza.obj");

    if (!object) {
        std::cout << Mesh::getError() << std::endl;
        return -1;
    }

    object->scale = {0.01f, 0.01f, 0.01f};
    object->position = {0, -1, 0};

    view->add(object);

    director->setRoot(view);

    director->setUpdateFunc(updateFunc);

    /*auto mesh = new Mesh(va, 12 * 3);

    director->setRoot(mesh);

    auto r = director->getRenderer();

    r->setProjection3D(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
    r->setView3D(glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));*/

    glfwSetFramebufferSizeCallback(window, window_resize);
    glfwSetCursorPosCallback(window, mouseMove);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while(!glfwWindowShouldClose(window)) {
        //std::cout << "VISIT!" << std::endl;
        director->visit();
        //std::cout << "UNVISIT!" << std::endl;

        /*Shader* sh = r->getShader("shape3d");

        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));

        r->pushTransform3D(model);

        r->renderObject3D(sh, va, 12 * 3);

        r->popTransform3D();*/
        
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    delete director;
    delete mat1;
    delete mat2;
    delete mat3;

    glfwTerminate();
    return 0;
}