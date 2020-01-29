//
//  coreTriangleArray.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 14.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef coreTriangleArray_hpp
#define coreTriangleArray_hpp

#include <stdio.h>

#include "object.hpp"
#include "coreTriangle.hpp"

class CoreTriangleCluster: public Object {
public:
    CoreTriangleCluster(Shader *shader, const RenderData *data, unsigned int size, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting);
    ~CoreTriangleCluster();
    
    Shader* getShaderPointer();
    Texture* getTexturePointer();
    
    unsigned int getSize();
    
    void render();
    
private:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    ArrayBuffer<glm::vec3> normal;
    
    unsigned int size;
    
    const glm::vec3 *vertices;
    const glm::vec2 *uvs;
    const glm::vec3 *normals;
    
    bool initLighting;
    
    Texture *texture;
    Texture *reflectionMap;
    int reflection;
    int useReflectionMap;
    
    Shader *shader;
    const RenderData *data;
    
    glm::mat4 *modelMatPointer;
    
    UniformVar<glm::mat4> model;
    UniformVar<glm::mat4> view;
    UniformVar<glm::mat4> projection;
    
    std::unique_ptr<UniformVar<int>> reflectionUniform;
    std::unique_ptr<UniformVar<int>> useReflectionMapUniform;
};

#endif /* coreTriangleArray_hpp */
