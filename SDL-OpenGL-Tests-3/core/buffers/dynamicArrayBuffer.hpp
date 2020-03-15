//
//  dynamicArrayBuffer.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef dynamicDynamicArrayBuffer_hpp
#define dynamicDynamicArrayBuffer_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

template <typename T>
class DynamicArrayBuffer {
public:
    DynamicArrayBuffer() = delete;
    ~DynamicArrayBuffer();
};


template<>
class DynamicArrayBuffer<GLuint> {
public:
    DynamicArrayBuffer(const GLuint *data, unsigned long dataSize, int shaderPos);
    DynamicArrayBuffer();
    ~DynamicArrayBuffer();
    
    void setData(const GLuint *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};


template<>
class DynamicArrayBuffer<GLfloat> {
public:
    DynamicArrayBuffer(const GLfloat *data, unsigned long dataSize, int shaderPos);
    DynamicArrayBuffer();
    ~DynamicArrayBuffer();
    
    void setData(const GLfloat *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};


template<>
class DynamicArrayBuffer<glm::vec2> {
public:
    DynamicArrayBuffer(const glm::vec2 *data, unsigned long dataSize, int shaderPos);
    DynamicArrayBuffer();
    ~DynamicArrayBuffer();
    
    void setData(const glm::vec2 *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};

template<>
class DynamicArrayBuffer<glm::vec3> {
public:
    DynamicArrayBuffer(const glm::vec3 *data, unsigned long dataSize, int shaderPos);
    DynamicArrayBuffer();
    ~DynamicArrayBuffer();
    
    void setData(const glm::vec3 *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};

template<>
class DynamicArrayBuffer<glm::vec4> {
public:
    DynamicArrayBuffer(const glm::vec4 *data, unsigned long dataSize, int shaderPos);
    DynamicArrayBuffer();
    ~DynamicArrayBuffer();
    
    void setData(const glm::vec4 *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};


#endif /* dynamicDynamicArrayBuffer_hpp */
