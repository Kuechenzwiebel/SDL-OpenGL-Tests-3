//
//  dynamicArrayBuffer.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "dynamicArrayBuffer.hpp"

DynamicArrayBuffer<GLuint>::DynamicArrayBuffer(const GLuint *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer<GLuint>::DynamicArrayBuffer() {
    
}

DynamicArrayBuffer<GLuint>::~DynamicArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void DynamicArrayBuffer<GLuint>::setData(const GLuint *data, unsigned long dataSize, int shaderPos) {
    shaderPos = shaderPos;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer<GLuint>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 1, GL_INT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for DynamicArrayBuffer not set!\n");
        return;
    }
}



DynamicArrayBuffer<GLfloat>::DynamicArrayBuffer(const GLfloat *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer<GLfloat>::DynamicArrayBuffer() {
    
}

DynamicArrayBuffer<GLfloat>::~DynamicArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void DynamicArrayBuffer<GLfloat>::setData(const GLfloat *data, unsigned long dataSize, int shaderPos) {
    shaderPos = shaderPos;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer<GLfloat>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for DynamicArrayBuffer not set!\n");
        return;
    }
}



DynamicArrayBuffer<glm::vec2>::DynamicArrayBuffer(const glm::vec2 *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer<glm::vec2>::DynamicArrayBuffer() {
    
}

DynamicArrayBuffer<glm::vec2>::~DynamicArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void DynamicArrayBuffer<glm::vec2>::setData(const glm::vec2 *data, unsigned long dataSize, int shaderPos) {
    shaderPos = shaderPos;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer<glm::vec2>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for DynamicArrayBuffer not set!\n");
        return;
    }
}



DynamicArrayBuffer<glm::vec3>::DynamicArrayBuffer(const glm::vec3 *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer<glm::vec3>::DynamicArrayBuffer() {
    
}

DynamicArrayBuffer<glm::vec3>::~DynamicArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void DynamicArrayBuffer<glm::vec3>::setData(const glm::vec3 *data, unsigned long dataSize, int shaderPos) {
    shaderPos = shaderPos;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer<glm::vec3>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for DynamicArrayBuffer not set!\n");
        return;
    }
}



DynamicArrayBuffer<glm::vec4>::DynamicArrayBuffer(const glm::vec4 *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

DynamicArrayBuffer<glm::vec4>::DynamicArrayBuffer() {
    
}

DynamicArrayBuffer<glm::vec4>::~DynamicArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void DynamicArrayBuffer<glm::vec4>::setData(const glm::vec4 *data, unsigned long dataSize, int shaderPos) {
    shaderPos = shaderPos;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

void DynamicArrayBuffer<glm::vec4>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for DynamicArrayBuffer not set!\n");
        return;
    }
}
