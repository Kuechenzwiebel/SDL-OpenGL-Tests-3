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

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "utils.hpp"
#include "arrayBuffers/arrayBuffer.hpp"

class CoreTriangle {
public:
    CoreTriangle(Shader *shader, const RenderData *data);
    ~CoreTriangle();
    
    virtual void prepareRender();
    void render();
    
private:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    
    Shader *shader;
    const RenderData *data;
};

#endif /* coreTriangle_hpp */
