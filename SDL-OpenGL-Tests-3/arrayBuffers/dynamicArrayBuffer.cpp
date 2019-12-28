//
//  dynamicArrayBuffer.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "dynamicArrayBuffer.hpp"

DynamicArrayBuffer::DynamicArrayBuffer(const GLfloat *data, int dataSize, int shaderPos, arrayBufferType type):
shaderPos(shaderPos), shaderSize(type), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer::DynamicArrayBuffer(const GLint *data, int dataSize, int shaderPos):
shaderPos(shaderPos), shaderSize(1), varType(GL_INT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer::DynamicArrayBuffer(const glm::vec3 *data, int dataSize, int shaderPos):
shaderPos(shaderPos), shaderSize(Vector3D), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer::DynamicArrayBuffer(const glm::vec2 *data, int dataSize, int shaderPos):
shaderPos(shaderPos), shaderSize(Vector2D), varType(GL_FLOAT) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer::DynamicArrayBuffer() {
    
}

void DynamicArrayBuffer::setData(const glm::vec3 *data, int dataSize, int shaderPos) {
    varType = GL_FLOAT;
    shaderPos = shaderPos;
    shaderSize = Vector3D;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicArrayBuffer::setData(const glm::vec2 *data, int dataSize, int shaderPos) {
    varType = GL_FLOAT;
    shaderPos = shaderPos;
    shaderSize = Vector2D;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer::setData(const GLfloat *data, int dataSize, int shaderPos, arrayBufferType type) {
    varType = GL_FLOAT;
    shaderPos = shaderPos;
    shaderSize = type;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer::changeData(const glm::vec3 *data, int dataSize, int shaderPos) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicArrayBuffer::changeData(const glm::vec2 *data, int dataSize, int shaderPos) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DynamicArrayBuffer::changeData(const GLfloat *data, int dataSize, int shaderPos, arrayBufferType type) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void DynamicArrayBuffer::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, shaderSize, varType, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for DynamicArrayBuffer not set!\n");
        return;
    }
}


DynamicArrayBuffer::~DynamicArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}
