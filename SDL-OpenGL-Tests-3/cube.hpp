//
//  cube.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 04.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef cube_hpp
#define cube_hpp

#include <stdio.h>
#include <array>

#include "object.hpp"
#include "texture.hpp"
#include "coreTriangle.hpp"

class Cube: public Object {
public:
    Cube(Shader *shader, const RenderData *data, Texture *texture);
    ~Cube();
    
    void addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles);
    
    void setModelMat(glm::mat4 model);
    
private:
    Shader *shader;
    const RenderData *data;
    Texture *texture;
    
    std::array<CoreTriangle, 12> tris;
};

#endif /* cube_hpp */
