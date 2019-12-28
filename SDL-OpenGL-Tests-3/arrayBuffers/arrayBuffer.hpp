//
//  arrayBuffer.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef arrayBuffer_hpp
#define arrayBuffer_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

static std::string dataSetErrorMessage = "Data for ArrayBuffer is allready set! Use a dynamic ArrayBuffer if you need to change the data!\n";

template <typename T>
class ArrayBuffer {
public:
    ArrayBuffer() = delete;
    ~ArrayBuffer();
};


template<>
class ArrayBuffer<GLuint> {
public:
    ArrayBuffer(const GLuint *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer();
    ~ArrayBuffer();
    
    void setData(const GLuint *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};


template<>
class ArrayBuffer<GLfloat> {
public:
    ArrayBuffer(const GLfloat *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer();
    ~ArrayBuffer();
    
    void setData(const GLfloat *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};


template<>
class ArrayBuffer<glm::vec2> {
public:
    ArrayBuffer(const glm::vec2 *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer();
    ~ArrayBuffer();
    
    void setData(const glm::vec2 *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};

template<>
class ArrayBuffer<glm::vec3> {
public:
    ArrayBuffer(const glm::vec3 *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer();
    ~ArrayBuffer();
    
    void setData(const glm::vec3 *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};

template<>
class ArrayBuffer<glm::vec4> {
public:
    ArrayBuffer(const glm::vec4 *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer();
    ~ArrayBuffer();
    
    void setData(const glm::vec4 *data, unsigned long dataSize, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int shaderPos;
};

#endif /* arrayBuffer_hpp */
