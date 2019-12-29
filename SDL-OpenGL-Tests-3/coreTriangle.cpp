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

CoreTriangle::CoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[]):
vertices(vertices), shader(shader), data(data), vertex(vertices, sizeof(glm::vec3) * 3, 0), modelMat(1), model(shader, "model", &modelMat), view(shader, "view", data->viewMat), projection(shader, "projection", data->projection) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    
    modelMat = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1), HALF_PI, glm::vec3(0.0f, 1.0f, 0.0f));
    
    glBindVertexArray(0);
}

CoreTriangle::~CoreTriangle() {
    glDeleteVertexArrays(1, &VAO);
}

void CoreTriangle::setModelMat(glm::mat4 model) {
    modelMat = model;
}

void CoreTriangle::prepareRender() {
    
}

void CoreTriangle::render() {
    prepareRender();
    
    vertex.activate();
    
    projection.setVar();
    view.setVar();
    model.setVar();
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

glm::vec3 CoreTriangle::getMaxVertex() {
    int i = 0;
    
    float distance1 = glm::length((modelMat * glm::vec4(vertices[0], 1.0f)).xyz());
    float distance2 = glm::length((modelMat * glm::vec4(vertices[1], 1.0f)).xyz());
    float distance3 = glm::length((modelMat * glm::vec4(vertices[2], 1.0f)).xyz());
    
    if(distance1 > distance2 && distance1 > distance3) {
        i = 0;
    }
    else if(distance2 > distance1 && distance2 > distance3) {
        i = 1;
    }
    else if(distance3 > distance1 && distance3 > distance2) {
        i = 2;
    }
    
    return triangleVertices[i];
}
