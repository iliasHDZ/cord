#include "framebuffer.hpp"

#include <glad/glad.h>

using namespace cord;

Framebuffer::Framebuffer(Renderer* renderer) : renderer(renderer) {
    init();
}

Framebuffer::~Framebuffer() {
    for (auto tex : textures)
        delete tex;

    glDeleteFramebuffers(1, &framebuffer);
}

void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture* Framebuffer::attachColor() {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer->getWidth(), renderer->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    return Texture::create(texture, renderer->getWidth(), renderer->getHeight(), 3);
}

Texture* Framebuffer::attachDepth() {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, renderer->getWidth(), renderer->getHeight(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

    return Texture::create(texture, renderer->getWidth(), renderer->getHeight(), 1);
}

void Framebuffer::init() {
    glGenFramebuffers(1, &framebuffer);
}