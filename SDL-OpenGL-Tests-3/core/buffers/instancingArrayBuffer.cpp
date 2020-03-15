//
//  instancingArrayBuffer.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "instancingArrayBuffer.hpp"

InstancedArrayBuffer::InstancedArrayBuffer(const glm::vec3 *data, int amount, int shaderPos):
shaderPos(shaderPos), amount(amount) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * amount, &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    dataSet = true;
}

InstancedArrayBuffer::InstancedArrayBuffer() {
    
}

InstancedArrayBuffer::~InstancedArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void InstancedArrayBuffer::setData(const glm::vec3 *data, int amount, int shaderPos) {
    if(dataSet == false) {
        amount = amount;
        shaderPos = shaderPos;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * amount, &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        dataSet = true;
    }
    else {
        printf("%s", dataSetErrorMessage.c_str());
        return;
    }
}

void InstancedArrayBuffer::activate() {
    if(dataSet) {
        glEnableVertexAttribArray(shaderPos);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(shaderPos, 3, GL_FLOAT, GL_FALSE, amount * sizeof(GLfloat), (GLvoid*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(shaderPos, 1);
    }
    else {
        printf("Data for InstancedArrayBuffer not set!\n");
        return;
    }
}
