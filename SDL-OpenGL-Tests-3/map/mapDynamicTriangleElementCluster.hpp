//
//  mapDynamicTriangleCluster.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 04.03.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef mapDynamicTriangleCluster_hpp
#define mapDynamicTriangleCluster_hpp

#include <stdio.h>

#include "../coreTriangleElementCluster.hpp"
#include "../core/buffers/dynamicArrayBuffer.hpp"

#include "mapDefines.hpp"

class MapDynamicTriangleElementCluster {
public:
    MapDynamicTriangleElementCluster(Shader *shader, const RenderData *data, unsigned int indicesSize, const MAP_INDICES_ARRAY_TYPE *indices, const glm::vec3 vertices[], unsigned int verticesSize, Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat);
    ~MapDynamicTriangleElementCluster();
    
    void setData(const MAP_INDICES_ARRAY_TYPE *indices, const glm::vec3 vertices[], const glm::vec2 uvs[], const glm::vec3 normals[]);
    
    void render();
    
private:
    GLuint VAO;
    DynamicArrayBuffer<glm::vec3> vertex;
    DynamicArrayBuffer<glm::vec2> texCoord;
    DynamicArrayBuffer<glm::vec3> normal;
    
    
    ElementArrayBuffer<MAP_INDICES_ARRAY_TYPE> elementArray;
    
    unsigned int indicesSize, verticesSize;
    
    const glm::vec3 *vertices;
    const glm::vec2 *uvs;
    const glm::vec3 *normals;
    const MAP_INDICES_ARRAY_TYPE *indices;
    
    Texture *texture;
    
    Shader *shader;
    const RenderData *data;
    
    glm::mat4 *modelMatPointer;
    
    UniformVar<glm::mat4> model;
};

#endif /* mapDynamicTriangleCluster_hpp */
