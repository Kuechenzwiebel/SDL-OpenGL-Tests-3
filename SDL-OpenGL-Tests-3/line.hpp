//
//  line.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 16.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef line_hpp
#define line_hpp

#include <stdio.h>

#include "object.hpp"
#include "core/buffers/dynamicArrayBuffer.hpp"

class Line {
public:
    Line(Shader *shader, const glm::vec3 vertices[], const glm::vec4 colors[]);
    ~Line();
    
    void setData(const glm::vec3 vertices[], const glm::vec4 colors[]);
    
    void render();
    
private:
    GLuint VAO;
    DynamicArrayBuffer<glm::vec3> vertex;
    DynamicArrayBuffer<glm::vec4> color;
    
    Shader *shader;
};

#endif /* line_hpp */
