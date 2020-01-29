//
//  coreTriangleArray.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 14.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "coreTriangleCluster.hpp"

CoreTriangleCluster::CoreTriangleCluster(Shader *shader, const RenderData *data, unsigned int size, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting):
size(size), vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data), modelMatPointer(modelMat),
vertex(vertices, sizeof(glm::vec3) * 3 * size, 0), texCoord(uvs, sizeof(glm::vec2) * 3 * size, 1), normal(normals, sizeof(glm::vec3) * 3 * size, 2),
model(shader, "model", modelMat), view(shader, "view", data->viewMat), projection(shader, "projection", data->projection), texture(texture), reflectionMap(reflectionMap), reflection(reflection), initLighting(initLighting) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    if(initLighting) {
        reflectionUniform = std::make_unique<UniformVar<int>>(shader, "reflection", &this->reflection);
        useReflectionMapUniform = std::make_unique<UniformVar<int>>(shader, "useReflectionMap", &this->useReflectionMap);
        
        if(reflectionMap == nullptr)
            useReflectionMap = false;
        else if(reflection == 0)
            useReflectionMap = true;
    }
    
    glBindVertexArray(0);
}

CoreTriangleCluster::~CoreTriangleCluster() {
    glDeleteVertexArrays(1, &VAO);
}

void CoreTriangleCluster::render() {
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
    glDrawArrays(GL_TRIANGLES, 0, 3 * size);
    glBindVertexArray(0);
}

Shader* CoreTriangleCluster::getShaderPointer() {
    return shader;
}

Texture* CoreTriangleCluster::getTexturePointer() {
    return texture;
}

unsigned int CoreTriangleCluster::getSize() {
    return size;
}
