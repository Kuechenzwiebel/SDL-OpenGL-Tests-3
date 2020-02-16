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

class Line {
public:
    Line(Shader *shader, const RenderData *data, const glm::vec3 vertices[], const glm::vec4 colors[]);
    ~Line();
    
    void render();
    
private:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec4> color;
    
    Shader *shader;
    const RenderData *data;
    
    UniformVar<glm::mat4> view;
    UniformVar<glm::mat4> projection;
};

#endif /* line_hpp */
