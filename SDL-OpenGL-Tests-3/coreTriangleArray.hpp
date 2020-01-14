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

class CoreTriangleArray: public Object {
public:
    CoreTriangleArray(Shader *shader, const RenderData *data, unsigned int size, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat);
    ~CoreTriangleArray();
    
    Shader* getShaderPointer();
    Texture* getTexturePointer();
    
    glm::vec3 getCenter();
    
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
    
    Texture *texture;
    Shader *shader;
    const RenderData *data;
    
    glm::mat4 *modelMatPointer;
    
    UniformVar<glm::mat4> model;
    UniformVar<glm::mat4> view;
    UniformVar<glm::mat4> projection;
};

#endif /* coreTriangleArray_hpp */
