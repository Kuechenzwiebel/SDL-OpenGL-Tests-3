//
//  instancingArrayBuffer.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef instancingArrayBuffer_hpp
#define instancingArrayBuffer_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "arrayBuffer.hpp"

class InstancedArrayBuffer {
public:
    InstancedArrayBuffer(const glm::vec3 *data, int amount, int shaderPos);
    InstancedArrayBuffer();
    ~InstancedArrayBuffer();
    
    void setData(const glm::vec3 *data, int amount, int shaderPos);
    
    void activate();
    
private:
    bool dataSet = false;
    
    GLuint buffer;
    int amount;
    int shaderPos;
};

#endif /* instancingArrayBuffer_hpp */
