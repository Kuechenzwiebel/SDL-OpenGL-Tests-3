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

enum arrayBufferType {
    Float = 1,
    Vector2D = 2,
    Vector3D = 3,
    Vector4D = 4
};

static std::string dataSetErrorMessage = "Data for ArrayBuffer is allready set! Use a dynamic ArrayBuffer if you need to change the data!\n";

class ArrayBuffer {
public:
    ArrayBuffer(const GLfloat *data, unsigned long dataSize, int shaderPos, arrayBufferType type);
    ArrayBuffer(const glm::vec4 *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer(const glm::vec3 *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer(const glm::vec2 *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer(const GLint *data, unsigned long dataSize, int shaderPos);
    ArrayBuffer();
    
    ArrayBuffer(const ArrayBuffer& other);
    ArrayBuffer& operator=(const ArrayBuffer& other);
    
    void setData(const GLfloat *data, unsigned long dataSize, int shaderPos, arrayBufferType type);
    void setData(const glm::vec4 *data, unsigned long dataSize, int shaderPos);
    void setData(const glm::vec3 *data, unsigned long dataSize, int shaderPos);
    void setData(const glm::vec2 *data, unsigned long dataSize, int shaderPos);
    
    ~ArrayBuffer();
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLenum varType;
    GLuint buffer;
    int shaderSize;
    int shaderPos;
};

#endif /* arrayBuffer_hpp */
