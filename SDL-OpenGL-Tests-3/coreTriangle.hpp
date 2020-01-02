//
//  coreTriangle.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef coreTriangle_hpp
#define coreTriangle_hpp

#include <stdio.h>

#include "object.hpp"

class CoreTriangle: public Object {
public:
    CoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[]);
    ~CoreTriangle();
    
    glm::vec3 getMaxVertex();
    
    void render();
    
protected:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    
    const glm::vec3 *vertices;
    
    Shader *shader;
    const RenderData *data;
    
    UniformVar<glm::mat4> model;
    UniformVar<glm::mat4> view;
    UniformVar<glm::mat4> projection;
};

#endif /* coreTriangle_hpp */
