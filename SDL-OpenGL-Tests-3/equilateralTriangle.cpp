//
//  equilateralTriangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 31.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "equilateralTriangle.hpp"

const static glm::vec3 equilateralTriangleVertices[] = {
    glm::vec3(-0.5f, -0.433f, 0.0f),
    glm::vec3(0.5f, -0.433f, 0.0f),
    glm::vec3(0.0f,  0.433f, 0.0f)
};

const static glm::vec2 equilateralTriangleUVs[] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.5f, 1.0f)
};

const static glm::vec3 equilateralTriangleNormals[] = {
    glm::triangleNormal(equilateralTriangleVertices[0], equilateralTriangleVertices[1], equilateralTriangleVertices[2]),  
    glm::triangleNormal(equilateralTriangleVertices[0], equilateralTriangleVertices[1], equilateralTriangleVertices[2]),
    glm::triangleNormal(equilateralTriangleVertices[0], equilateralTriangleVertices[1], equilateralTriangleVertices[2])
};

EquilateralTriangle::EquilateralTriangle(Shader *shader, const RenderData *data, Texture *texture):
CoreTriangle(shader, data, equilateralTriangleVertices, texture, equilateralTriangleUVs, equilateralTriangleNormals) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    
    glBindVertexArray(0);
}

EquilateralTriangle::~EquilateralTriangle() {
    glDeleteVertexArrays(1, &VAO);
}
