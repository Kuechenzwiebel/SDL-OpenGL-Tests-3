//
//  coreTriangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "coreTriangle.hpp"

CoreTriangle::CoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting):
vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data), modelMatPointer(modelMat), reflection(reflection), reflectionMap(reflectionMap), texture(texture), useReflectionMap(false),
vertex(vertices, sizeof(glm::vec3) * 3, 0), texCoord(uvs, sizeof(glm::vec2) * 3, 1), normal(normals, sizeof(glm::vec3) * 3, 2),
model(shader, "model", modelMat), view(shader, "view", data->viewMat), projection(shader, "projection", data->projection), initLighting(initLighting) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    if(initLighting) {
        reflectionUniform = std::make_unique<UniformVar<int>>(shader, "reflection", &reflection);
        useReflectionMapUniform = std::make_unique<UniformVar<int>>(shader, "useReflectionMap", &useReflectionMap);
        
        if(reflectionMap == nullptr)
            useReflectionMap = false;
        else if(reflection == 0)
            useReflectionMap = true;
    }
    
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
    texCoord.activate();
    normal.activate();
    
    projection.setVar();
    view.setVar();
    model.setVar();
    
    if(initLighting) {
        useReflectionMapUniform->setVar();
        
        if(useReflectionMap) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, reflectionMap->getTextureID());
        }
        else {
            reflectionUniform->setVar();
        }
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

glm::vec3 CoreTriangle::getCenter() {
    return (*modelMatPointer * glm::vec4(vertices[0], 1.0f) + *modelMatPointer * glm::vec4(vertices[1], 1.0f) + *modelMatPointer * glm::vec4(vertices[2], 1.0f)) / 3.0f;
}

Shader* CoreTriangle::getShaderPointer() {
    return shader;
}

Texture* CoreTriangle::getTexturePointer() {
    return texture;
}


bool operator<(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r) {
    if(l.first < r.first)
        return true;
    else
        return false;
}

bool operator>(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r) {
    if(l.first > r.first)
        return true;
    else
        return false;
}

bool operator==(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r) {
    if(l.first == r.first)
        return true;
    else
        return false;
}
