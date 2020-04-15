//
//  elementArrayBuffer.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 15.04.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "elementArrayBuffer.hpp"

ElementArrayBuffer<unsigned char>::ElementArrayBuffer() {
    
}

ElementArrayBuffer<unsigned char>::~ElementArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void ElementArrayBuffer<unsigned char>::setData(const unsigned char *data, unsigned int dataSize) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
}



ElementArrayBuffer<unsigned short>::ElementArrayBuffer() {
    
}

ElementArrayBuffer<unsigned short>::~ElementArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void ElementArrayBuffer<unsigned short>::setData(const unsigned short *data, unsigned int dataSize) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
}



ElementArrayBuffer<unsigned int>::ElementArrayBuffer() {
    
}

ElementArrayBuffer<unsigned int>::~ElementArrayBuffer() {
    glDeleteBuffers(1, &buffer);
}

void ElementArrayBuffer<unsigned int>::setData(const unsigned int *data, unsigned int dataSize) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
}
