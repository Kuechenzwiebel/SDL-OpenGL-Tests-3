//
//  uniformBuffer.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 15.03.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef uniformBuffer_hpp
#define uniformBuffer_hpp

#include <stdio.h>

#include "../shader.hpp"

class UniformBuffer {
public:
    UniformBuffer(int bufferSize, int binding);
    ~UniformBuffer();
    
    void modifyData(int size, int offset, float *data);
    void addToShader(Shader *shader, std::string name, int binding);
    
private:
    GLuint buffer;
};

#endif /* uniformBuffer_hpp */
