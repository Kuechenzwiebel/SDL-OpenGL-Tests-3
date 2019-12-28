//
//  dynamicArrayBuffer.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef dynamicArrayBuffer_hpp
#define dynamicArrayBuffer_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "arrayBuffer.hpp"

class DynamicArrayBuffer {
public:
    DynamicArrayBuffer(const GLfloat *data, int dataSize, int shaderPos, arrayBufferType type);
    DynamicArrayBuffer(const glm::vec3 *data, int dataSize, int shaderPos);
    DynamicArrayBuffer(const glm::vec2 *data, int dataSize, int shaderPos);
    DynamicArrayBuffer(const GLint *data, int dataSize, int shaderPos);
    DynamicArrayBuffer();
    
    void setData(const glm::vec3 *data, int dataSize, int shaderPos);
    void setData(const glm::vec2 *data, int dataSize, int shaderPos);
    void setData(const GLfloat *data, int dataSize, int shaderPos, arrayBufferType type);
    
    void changeData(const glm::vec3 *data, int dataSize, int shaderPos);
    void changeData(const glm::vec2 *data, int dataSize, int shaderPos);
    void changeData(const GLfloat *data, int dataSize, int shaderPos, arrayBufferType type);
    
    ~DynamicArrayBuffer();
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLenum varType;
    GLuint buffer;
    int shaderSize;
    int shaderPos;
};

#endif /* dynamicArrayBuffer_hpp */
