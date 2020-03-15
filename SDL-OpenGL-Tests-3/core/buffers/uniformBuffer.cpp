//
//  uniformBuffer.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 15.03.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "uniformBuffer.hpp"

UniformBuffer::UniformBuffer(int bufferSize, int binding) {
    glGenBuffers(1, &buffer);
      
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
      
    glBindBufferRange(GL_UNIFORM_BUFFER, binding, buffer, 0, bufferSize);
}

UniformBuffer::~UniformBuffer() {
    glDeleteBuffers(1, &buffer);
}
   
void UniformBuffer::modifyData(int size, int offset, float *data) {
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::addToShader(Shader *shader, std::string name, int binding) {
    glUniformBlockBinding(shader->getProgram(), glGetUniformBlockIndex(shader->getProgram(), name.c_str()), binding);
}
