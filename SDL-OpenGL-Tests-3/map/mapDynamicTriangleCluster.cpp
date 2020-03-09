//
//  mapDynamicTriangleCluster.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 04.03.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "mapDynamicTriangleCluster.hpp"

MapDynamicTriangleCluster::MapDynamicTriangleCluster(Shader *shader, const RenderData *data, unsigned int size, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat):
size(size), vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data), modelMatPointer(modelMat),
vertex(vertices, sizeof(glm::vec3) * 3 * size, 0), texCoord(uvs, sizeof(glm::vec2) * 3 * size, 1), normal(normals, sizeof(glm::vec3) * 3 * size, 2),
model(shader, "model", modelMat), view(shader, "view", data->viewMat), projection(shader, "projection", data->projection), texture(texture) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    glBindVertexArray(0);
}

MapDynamicTriangleCluster::~MapDynamicTriangleCluster() {
    glDeleteVertexArrays(1, &VAO);
}

void MapDynamicTriangleCluster::render() {
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    projection.setVar();
    view.setVar();
    model.setVar();
    
    texture->activate(shader, 0);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3 * size);
    glBindVertexArray(0);
}

void MapDynamicTriangleCluster::setData(unsigned int size, const glm::vec3 vertices[], const glm::vec2 uvs[], const glm::vec3 normals[]) {
    vertex.setData(vertices, sizeof(glm::vec3) * 3 * size, 0);
    texCoord.setData(uvs, sizeof(glm::vec2) * 3 * size, 1);
    normal.setData(normals, sizeof(glm::vec3) * 3 * size, 2);
    this->size = size;
}

Shader* MapDynamicTriangleCluster::getShaderPointer() {
    return shader;
}

Texture* MapDynamicTriangleCluster::getTexturePointer() {
    return texture;
}

unsigned int MapDynamicTriangleCluster::getSize() {
    return size;
}
