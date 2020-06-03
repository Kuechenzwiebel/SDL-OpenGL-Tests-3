//
//  uiRectangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 11.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "uiRectangle.hpp"

static glm::vec3 uiRectangleVertices[2][3] = {
    {   glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3( 0.5f, -0.5f, 0.0f),
        glm::vec3(-0.5f,  0.5f, 0.0f)},
    
    {   glm::vec3( 0.5f,  0.5f, 0.0f),
        glm::vec3( 0.5f, -0.5f, 0.0f),
        glm::vec3(-0.5f,  0.5f, 0.0f)}
};

static glm::vec2 uiRectangleUVs[2][3] = {
    {   glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)},
    
    {   glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)}
};

static glm::vec3 uiRectangleNormals[] = {
    glm::vec3(0.0f),
    glm::vec3(0.0f),
    glm::vec3(0.0f)
};

UIRectangle::UIRectangle(Shader *shader, Texture *texture):
shader(shader), texture(texture),
tri1(shader, uiRectangleVertices[0], texture, uiRectangleUVs[0], uiRectangleNormals, &modelMat, &offset),
tri2(shader, uiRectangleVertices[1], texture, uiRectangleUVs[1], uiRectangleNormals, &modelMat, &offset) {
    
}


UIRectangle::UIRectangle(Shader *shader, Texture *texture, const glm::vec2 customUVs[2][3]):
shader(shader), texture(texture),
tri1(shader, uiRectangleVertices[0], texture, &customUVs[0][0], uiRectangleNormals, &modelMat, &offset),
tri2(shader, uiRectangleVertices[1], texture, &customUVs[1][0], uiRectangleNormals, &modelMat, &offset) {
}

void UIRectangle::addToTriangleList(std::vector<CoreTriangle*> *triangles) {
    tri1.addToTriangleList(triangles);
    tri2.addToTriangleList(triangles);
}

void UIRectangle::setTextureOffset(glm::vec2 offset) {
    this->offset = offset;
}

glm::vec2 UIRectangle::getTextureOffset() {
    return offset;
}
