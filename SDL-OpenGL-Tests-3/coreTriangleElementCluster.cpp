//
//  coreTriangleElementCluster.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 15.04.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "coreTriangleElementCluster.hpp"

CoreTriangleElementCluster<unsigned char>::CoreTriangleElementCluster(Shader *shader, const RenderData *data, unsigned int indicesSize, const unsigned char *indices, const glm::vec3 vertices[], unsigned int verticesSize, Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting):

CoreTriangleCluster(shader, data, verticesSize, vertices, texture, uvs, normals, modelMat, reflection, reflectionMap, initLighting),

indicesSize(indicesSize), vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data), modelMatPointer(modelMat),

vertex(vertices, sizeof(glm::vec3) * verticesSize, 0), texCoord(uvs, sizeof(glm::vec2) * verticesSize, 1), normal(normals, sizeof(glm::vec3) * verticesSize, 2),

model(shader, "model", modelMat), texture(texture), reflectionMap(reflectionMap), reflection(reflection), initLighting(initLighting), indices(indices), verticesSize(verticesSize) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    elementArray.setData(indices, indicesSize * sizeof(unsigned char));
    
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

CoreTriangleElementCluster<unsigned char>::~CoreTriangleElementCluster() {
    glDeleteVertexArrays(1, &VAO);
}

void CoreTriangleElementCluster<unsigned char>::render() {
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    model.setVar();
    
    if(initLighting) {
        useReflectionMapUniform->setVar();
        
        if(useReflectionMap) {
            reflectionMap->activate(shader, 1);
        }
        else {
            reflectionUniform->setVar();
        }
    }
    
    texture->activate(shader, 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_BYTE, 0);
    glBindVertexArray(0);
}



CoreTriangleElementCluster<unsigned short>::CoreTriangleElementCluster(Shader *shader, const RenderData *data, unsigned int indicesSize, const unsigned short *indices, const glm::vec3 vertices[], unsigned int verticesSize, Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting):

CoreTriangleCluster(shader, data, verticesSize, vertices, texture, uvs, normals, modelMat, reflection, reflectionMap, initLighting),

indicesSize(indicesSize), vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data), modelMatPointer(modelMat),

vertex(vertices, sizeof(glm::vec3) * verticesSize, 0), texCoord(uvs, sizeof(glm::vec2) * verticesSize, 1), normal(normals, sizeof(glm::vec3) * verticesSize, 2),

model(shader, "model", modelMat), texture(texture), reflectionMap(reflectionMap), reflection(reflection), initLighting(initLighting), indices(indices), verticesSize(verticesSize) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    elementArray.setData(indices, indicesSize * sizeof(unsigned short));
    
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

CoreTriangleElementCluster<unsigned short>::~CoreTriangleElementCluster() {
    glDeleteVertexArrays(1, &VAO);
}

void CoreTriangleElementCluster<unsigned short>::render() {
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    model.setVar();
    
    if(initLighting) {
        useReflectionMapUniform->setVar();
        
        if(useReflectionMap) {
            reflectionMap->activate(shader, 1);
        }
        else {
            reflectionUniform->setVar();
        }
    }
    
    texture->activate(shader, 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}



CoreTriangleElementCluster<unsigned int>::CoreTriangleElementCluster(Shader *shader, const RenderData *data, unsigned int indicesSize, const unsigned int *indices, const glm::vec3 vertices[], unsigned int verticesSize, Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting):

CoreTriangleCluster(shader, data, verticesSize, vertices, texture, uvs, normals, modelMat, reflection, reflectionMap, initLighting),

indicesSize(indicesSize), vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data), modelMatPointer(modelMat),

vertex(vertices, sizeof(glm::vec3) * verticesSize, 0), texCoord(uvs, sizeof(glm::vec2) * verticesSize, 1), normal(normals, sizeof(glm::vec3) * verticesSize, 2),

model(shader, "model", modelMat), texture(texture), reflectionMap(reflectionMap), reflection(reflection), initLighting(initLighting), indices(indices), verticesSize(verticesSize) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    elementArray.setData(indices, indicesSize * sizeof(unsigned int));
    
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

CoreTriangleElementCluster<unsigned int>::~CoreTriangleElementCluster() {
    glDeleteVertexArrays(1, &VAO);
}

void CoreTriangleElementCluster<unsigned int>::render() {
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    model.setVar();
    
    if(initLighting) {
        useReflectionMapUniform->setVar();
        
        if(useReflectionMap) {
            reflectionMap->activate(shader, 1);
        }
        else {
            reflectionUniform->setVar();
        }
    }
    
    texture->activate(shader, 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
