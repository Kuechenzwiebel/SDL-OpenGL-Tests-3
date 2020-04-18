//
//  mapDynamicTriangleCluster.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 04.03.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "mapDynamicTriangleElementCluster.hpp"

MapDynamicTriangleElementCluster::MapDynamicTriangleElementCluster(Shader *shader, const RenderData *data, unsigned int indicesSize, const MAP_INDICES_ARRAY_TYPE *indices, const glm::vec3 vertices[], unsigned int verticesSize, Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat):
verticesSize(verticesSize), indicesSize(indicesSize), vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data), modelMatPointer(modelMat),
vertex(vertices, sizeof(glm::vec3) * verticesSize, 0), texCoord(uvs, sizeof(glm::vec2) * verticesSize, 1), normal(normals, sizeof(glm::vec3) * verticesSize, 2),
model(shader, "model", modelMat), texture(texture), indices(indices)  {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    elementArray.setData(indices, indicesSize * sizeof(MAP_INDICES_ARRAY_TYPE));
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    glBindVertexArray(0);
}

MapDynamicTriangleElementCluster::~MapDynamicTriangleElementCluster() {
    glDeleteVertexArrays(1, &VAO);
}

void MapDynamicTriangleElementCluster::render() {
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    model.setVar();
    
    texture->activate(shader, 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, MAP_INDICES_ARRAY_GL_TYPE, 0);
    glBindVertexArray(0);
}

void MapDynamicTriangleElementCluster::setData(const MAP_INDICES_ARRAY_TYPE *indices, const glm::vec3 vertices[], const glm::vec2 uvs[], const glm::vec3 normals[]) {
    vertex.setData(vertices, sizeof(glm::vec3) * verticesSize, 0);
    texCoord.setData(uvs, sizeof(glm::vec2) * verticesSize, 1);
    normal.setData(normals, sizeof(glm::vec3) * verticesSize, 2);
}
