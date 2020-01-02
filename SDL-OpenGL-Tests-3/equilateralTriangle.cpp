//
//  equilateralTriangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 31.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "equilateralTriangle.hpp"

glm::vec3 eTriangleVertices[] = {
    glm::vec3(-0.5f, -0.433f, 0.0f),
    glm::vec3(0.5f, -0.433f, 0.0f),
    glm::vec3(0.0f,  0.433f, 0.0f)
};

EquilateralTriangle::EquilateralTriangle(Shader *shader, const RenderData *data):
CoreTriangle(shader, data, eTriangleVertices) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    
    glBindVertexArray(0);
}

EquilateralTriangle::~EquilateralTriangle() {
    glDeleteVertexArrays(1, &VAO);
}
