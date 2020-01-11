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

UIRectangle::UIRectangle(Shader *shader, const RenderData *data, Texture *texture):
shader(shader), data(data), texture(texture),
tri1(shader, data, uiRectangleVertices[0], texture, uiRectangleUVs[0], uiRectangleNormals),
tri2(shader, data, uiRectangleVertices[1], texture, uiRectangleUVs[1], uiRectangleNormals) {
    
}

void UIRectangle::addToTriangleList(std::vector<CoreTriangle*> *triangles) {
    triangles->push_back(&tri1);
    triangles->push_back(&tri2);
}

void UIRectangle::setModelMat(glm::mat4 model) {
    tri1.setModelMat(model);
    tri2.setModelMat(model);
}

void UIRectangle::calculateModelMat() {
    modelMat = glm::translate(glm::mat4(1), translation) * glm::rotate(glm::mat4(1), rotation.w, rotation.xyz()) * glm::scale(glm::mat4(1), scale);
    setModelMat(modelMat);
}
