//
//  coreTriangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "coreTriangle.hpp"

CoreTriangle::CoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[]):
vertices(vertices), shader(shader), data(data), vertex(vertices, sizeof(glm::vec3) * 3, 0), translation(0.0f), scale(1.0f), rotation(0.0f), modelMat(1), model(shader, "model", &modelMat), view(shader, "view", data->viewMat), projection(shader, "projection", data->projection) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    
    glBindVertexArray(0);
}

CoreTriangle::~CoreTriangle() {
    glDeleteVertexArrays(1, &VAO);
}

void CoreTriangle::setTranslation(glm::vec3 translation) {
    this->translation = translation;
    calculateModelMat();
}

void CoreTriangle::setRotation(glm::vec4 rotation) {
    this->rotation = rotation;
    calculateModelMat();
}

void CoreTriangle::setScale(glm::vec3 scale) {
    this->scale = scale;
    calculateModelMat();
}
   
void CoreTriangle::setModelMat(glm::mat4 model) {
    modelMat = model;
}
   
glm::vec3 CoreTriangle::getTranslation() {
    return translation;
}

glm::vec4 CoreTriangle::getRotation() {
    return rotation;
}

glm::vec3 CoreTriangle::getScale() {
    return scale;
}

glm::mat4 CoreTriangle::getModelMat() {
    return modelMat;
}

void CoreTriangle::calculateModelMat() {
    modelMat = glm::translate(glm::mat4(1), translation) * glm::rotate(glm::mat4(1), rotation.w, rotation.xyz()) * glm::scale(glm::mat4(1), scale);
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
    
    return (modelMat * glm::vec4(vertices[i], 1.0f)).xyz();
}



bool operator<(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r) {
    if(l.first < r.first) {
        return true;
    }
    else {
        return false;
    }
}

bool operator>(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r) {
    if(l.first > r.first) {
        return true;
    }
    else {
        return false;
    }
}
