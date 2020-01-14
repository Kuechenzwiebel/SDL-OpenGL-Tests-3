//
//  coreTriangleArray.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 14.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "coreTriangleArray.hpp"

CoreTriangleArray::CoreTriangleArray(Shader *shader, const RenderData *data, unsigned int size, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat):
size(size), vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data), modelMatPointer(modelMat),
vertex(vertices, sizeof(glm::vec3) * 3 * size, 0), texCoord(uvs, sizeof(glm::vec2) * 3 * size, 1), normal(normals, sizeof(glm::vec3) * 3 * size, 2),
model(shader, "model", modelMat), view(shader, "view", data->viewMat), projection(shader, "projection", data->projection), texture(texture){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    glBindVertexArray(0);
}

CoreTriangleArray::~CoreTriangleArray() {
    glDeleteVertexArrays(1, &VAO);
}
   
void CoreTriangleArray::render() {
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    projection.setVar();
    view.setVar();
    model.setVar();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3 * size);
    glBindVertexArray(0);
}

Shader* CoreTriangleArray::getShaderPointer() {
    return shader;
}

Texture* CoreTriangleArray::getTexturePointer() {
    return texture;
}

glm::vec3 CoreTriangleArray::getCenter() {
    return glm::vec3(0.0f);
}

unsigned int CoreTriangleArray::getSize() {
    return size;
}
