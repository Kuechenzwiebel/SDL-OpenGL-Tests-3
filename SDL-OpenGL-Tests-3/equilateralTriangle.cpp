//
//  equilateralTriangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 31.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "equilateralTriangle.hpp"

static glm::vec3 equilateralTriangleVertices[] = {
    glm::vec3(-0.5f, -0.433f, 0.0f),
    glm::vec3(0.5f, -0.433f, 0.0f),
    glm::vec3(0.0f,  0.433f, 0.0f)
};

static glm::vec2 equilateralTriangleUVs[] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.5f, 1.0f)
};

static glm::vec3 equilateralTriangleNormals[] = {
    glm::triangleNormal(equilateralTriangleVertices[0], equilateralTriangleVertices[1], equilateralTriangleVertices[2]),  
    glm::triangleNormal(equilateralTriangleVertices[0], equilateralTriangleVertices[1], equilateralTriangleVertices[2]),
    glm::triangleNormal(equilateralTriangleVertices[0], equilateralTriangleVertices[1], equilateralTriangleVertices[2])
};

EquilateralTriangle::EquilateralTriangle(Shader *shader, Texture *texture, int reflection, Texture *reflectionMap, bool initLighting):
CoreTriangle(shader, equilateralTriangleVertices, texture, equilateralTriangleUVs, equilateralTriangleNormals, &modelMat, reflection, reflectionMap, initLighting), trianglePointer(nullptr) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    if(reflectionMap == nullptr)
        useReflectionMap = false;
    else if(reflection == 0)
        useReflectionMap = true;
    
    glBindVertexArray(0);
}

EquilateralTriangle::~EquilateralTriangle() {
    glDeleteVertexArrays(1, &VAO);
    if(trianglePointer != nullptr) {
        auto findIter = std::find_if(trianglePointer->begin(), trianglePointer->end(), [this](std::pair<float, CoreTriangle*> p){return p.second == this;});
        if(findIter != trianglePointer->end()) {
            trianglePointer->erase(findIter);
        }
    }
}

void EquilateralTriangle::addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles) {
    triangles->push_back(std::make_pair(0.0f, this));
    trianglePointer = triangles;
}
