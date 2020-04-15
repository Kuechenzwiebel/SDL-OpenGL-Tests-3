//
//  elementArrayBuffer.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 15.04.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef elementArrayBuffer_hpp
#define elementArrayBuffer_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

template <typename T>
class ElementArrayBuffer {
public:
    ElementArrayBuffer() = delete;
    ~ElementArrayBuffer();
};

template <>
class ElementArrayBuffer<unsigned char> {
public:
    ElementArrayBuffer();
    ~ElementArrayBuffer();
    
    void setData(const unsigned char *data, unsigned int dataSize);
    
private:
    GLuint buffer;
};

template <>
class ElementArrayBuffer<unsigned short> {
public:
    ElementArrayBuffer();
    ~ElementArrayBuffer();
    
    void setData(const unsigned short *data, unsigned int dataSize);
    
private:
    GLuint buffer;
};

template <>
class ElementArrayBuffer<unsigned int> {
public:
    ElementArrayBuffer();
    ~ElementArrayBuffer();
    
    void setData(const unsigned int *data, unsigned int dataSize);
    
private:
    GLuint buffer;
};


#endif /* elementArrayBuffer_hpp */
