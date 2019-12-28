//
//  coreTriangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "coreTriangle.hpp"

static glm::vec3 triangleVertices[] = {
    glm::vec3(-0.5f, -0.5f, 0.0f),
    glm::vec3(0.5f, -0.5f, 0.0f),
    glm::vec3(0.0f,  0.5f, 0.0f)
};

CoreTriangle::CoreTriangle(Shader *shader, const RenderData *data):
shader(shader), data(data), vertex(triangleVertices, sizeof(triangleVertices), 0) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    
    glBindVertexArray(0);
}

CoreTriangle::~CoreTriangle() {
    glDeleteVertexArrays(1, &VAO);
}

void CoreTriangle::prepareRender() {
    
}

void CoreTriangle::render() {
    prepareRender();
    
    vertex.activate();
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
