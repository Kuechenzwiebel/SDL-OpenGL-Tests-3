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

#include "../coreTriangleCluster.hpp"
#include "../core/buffers/dynamicArrayBuffer.hpp"

class MapDynamicTriangleCluster {
public:
    MapDynamicTriangleCluster(Shader *shader, const RenderData *data, unsigned int size, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat);
    ~MapDynamicTriangleCluster();
    
    Shader* getShaderPointer();
    Texture* getTexturePointer();
    
    unsigned int getSize();
    
    void setData(unsigned int size, const glm::vec3 vertices[], const glm::vec2 uvs[], const glm::vec3 normals[]);
    
    void render();
    
private:
    GLuint VAO;
    DynamicArrayBuffer<glm::vec3> vertex;
    DynamicArrayBuffer<glm::vec2> texCoord;
    DynamicArrayBuffer<glm::vec3> normal;
    
    unsigned int size;
    
    const glm::vec3 *vertices;
    const glm::vec2 *uvs;
    const glm::vec3 *normals;
    
    Texture *texture;
    
    Shader *shader;
    const RenderData *data;
    
    glm::mat4 *modelMatPointer;
    
    UniformVar<glm::mat4> model;
};

#endif /* mapDynamicTriangleCluster_hpp */
