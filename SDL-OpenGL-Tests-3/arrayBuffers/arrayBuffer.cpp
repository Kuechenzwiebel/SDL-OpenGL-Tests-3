//
//  arrayBuffer.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "arrayBuffer.hpp"

ArrayBuffer<GLuint>::ArrayBuffer(const GLuint *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer<GLuint>::ArrayBuffer() {
    
}

ArrayBuffer<GLuint>::~ArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void ArrayBuffer<GLuint>::setData(const GLuint *data, unsigned long dataSize, int shaderPos) {
    if(dataSet == false) {
        shaderPos = shaderPos;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
    }
}

void ArrayBuffer<GLuint>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 1, GL_INT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for ArrayBuffer not set!\n");
        return;
    }
}



ArrayBuffer<GLfloat>::ArrayBuffer(const GLfloat *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer<GLfloat>::ArrayBuffer() {
    
}

ArrayBuffer<GLfloat>::~ArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void ArrayBuffer<GLfloat>::setData(const GLfloat *data, unsigned long dataSize, int shaderPos) {
    if(dataSet == false) {
        shaderPos = shaderPos;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
    }
}

void ArrayBuffer<GLfloat>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for ArrayBuffer not set!\n");
        return;
    }
}



ArrayBuffer<glm::vec2>::ArrayBuffer(const glm::vec2 *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer<glm::vec2>::ArrayBuffer() {
    
}

ArrayBuffer<glm::vec2>::~ArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void ArrayBuffer<glm::vec2>::setData(const glm::vec2 *data, unsigned long dataSize, int shaderPos) {
    if(dataSet == false) {
        shaderPos = shaderPos;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
    }
}

void ArrayBuffer<glm::vec2>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for ArrayBuffer not set!\n");
        return;
    }
}



ArrayBuffer<glm::vec3>::ArrayBuffer(const glm::vec3 *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer<glm::vec3>::ArrayBuffer() {
    
}

ArrayBuffer<glm::vec3>::~ArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void ArrayBuffer<glm::vec3>::setData(const glm::vec3 *data, unsigned long dataSize, int shaderPos) {
    if(dataSet == false) {
        shaderPos = shaderPos;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
    }
}

void ArrayBuffer<glm::vec3>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for ArrayBuffer not set!\n");
        return;
    }
}



ArrayBuffer<glm::vec4>::ArrayBuffer(const glm::vec4 *data, unsigned long dataSize, int shaderPos):
shaderPos(shaderPos) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

ArrayBuffer<glm::vec4>::ArrayBuffer() {
    
}

ArrayBuffer<glm::vec4>::~ArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void ArrayBuffer<glm::vec4>::setData(const glm::vec4 *data, unsigned long dataSize, int shaderPos) {
    if(dataSet == false) {
        shaderPos = shaderPos;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
    }
}

void ArrayBuffer<glm::vec4>::activate() {
    if(dataSet == true) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(shaderPos);
        glVertexAttribPointer(shaderPos, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    }
    else {
        printf("Data for ArrayBuffer not set!\n");
        return;
    }
}
