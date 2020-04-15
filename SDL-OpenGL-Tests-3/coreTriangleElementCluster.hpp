//
//  coreTriangleElementCluster.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 15.04.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef coreTriangleElementCluster_hpp
#define coreTriangleElementCluster_hpp

#include <stdio.h>
#include <type_traits>

#include "object.hpp"
#include "coreTriangleCluster.hpp"
#include "core/buffers/elementArrayBuffer.hpp"

template <typename T>
class CoreTriangleElementCluster {
    CoreTriangleElementCluster() = delete;
    ~CoreTriangleElementCluster();
};

template<>
class CoreTriangleElementCluster<unsigned char>: public CoreTriangleCluster {
public:
    CoreTriangleElementCluster(Shader *shader, const RenderData *data, unsigned int indicesSize, const unsigned char *indices, const glm::vec3 vertices[], unsigned int verticesSize, Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting);
    ~CoreTriangleElementCluster();
    
    void render();
    
private:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    ArrayBuffer<glm::vec3> normal;
    
    
    ElementArrayBuffer<unsigned char> elementArray;
    
    unsigned int indicesSize, verticesSize;
    
    const glm::vec3 *vertices;
    const glm::vec2 *uvs;
    const glm::vec3 *normals;
    const unsigned char *indices;
    
    bool initLighting;
    
    Texture *texture;
    Texture *reflectionMap;
    int reflection;
    int useReflectionMap;
    
    Shader *shader;
    const RenderData *data;
    
    glm::mat4 *modelMatPointer;
    
    UniformVar<glm::mat4> model;
    
    std::unique_ptr<UniformVar<int>> reflectionUniform;
    std::unique_ptr<UniformVar<int>> useReflectionMapUniform;
};



template<>
class CoreTriangleElementCluster<unsigned short>: public CoreTriangleCluster {
public:
    CoreTriangleElementCluster(Shader *shader, const RenderData *data, unsigned int indicesSize, const unsigned short *indices, const glm::vec3 vertices[], unsigned int verticesSize, Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting);
    ~CoreTriangleElementCluster();
    
    void render();
    
private:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    ArrayBuffer<glm::vec3> normal;
    
    
    ElementArrayBuffer<unsigned short> elementArray;
    
    unsigned int indicesSize, verticesSize;
    
    const glm::vec3 *vertices;
    const glm::vec2 *uvs;
    const glm::vec3 *normals;
    const unsigned short *indices;
    
    bool initLighting;
    
    Texture *texture;
    Texture *reflectionMap;
    int reflection;
    int useReflectionMap;
    
    Shader *shader;
    const RenderData *data;
    
    glm::mat4 *modelMatPointer;
    
    UniformVar<glm::mat4> model;
    
    std::unique_ptr<UniformVar<int>> reflectionUniform;
    std::unique_ptr<UniformVar<int>> useReflectionMapUniform;
};



template<>
class CoreTriangleElementCluster<unsigned int>: public CoreTriangleCluster {
public:
    CoreTriangleElementCluster(Shader *shader, const RenderData *data, unsigned int indicesSize, const unsigned int *indices, const glm::vec3 vertices[], unsigned int verticesSize, Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting);
    ~CoreTriangleElementCluster();
    
    void render();
    
private:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    ArrayBuffer<glm::vec3> normal;
    
    
    ElementArrayBuffer<unsigned int> elementArray;
    
    unsigned int indicesSize, verticesSize;
    
    const glm::vec3 *vertices;
    const glm::vec2 *uvs;
    const glm::vec3 *normals;
    const unsigned int *indices;
    
    bool initLighting;
    
    Texture *texture;
    Texture *reflectionMap;
    int reflection;
    int useReflectionMap;
    
    Shader *shader;
    const RenderData *data;
    
    glm::mat4 *modelMatPointer;
    
    UniformVar<glm::mat4> model;
    
    std::unique_ptr<UniformVar<int>> reflectionUniform;
    std::unique_ptr<UniformVar<int>> useReflectionMapUniform;
};

#endif /* coreTriangleElementCluster_hpp */
