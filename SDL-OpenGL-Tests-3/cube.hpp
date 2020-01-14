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
#include "coreTriangleArray.hpp"

class Cube: public Object {
public:
    Cube(Shader *shader, const RenderData *data, Texture *texture);
    ~Cube();
    
    void addToTriangleList(std::vector<CoreTriangleArray*> *triangles);
    
private:    
    CoreTriangleArray tris;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
};

#endif /* cube_hpp */
