//
//  equilateralTriangle.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 31.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef equilateralTriangle_hpp
#define equilateralTriangle_hpp

#include <stdio.h>

#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>

#include "coreTriangle.hpp"
#include "object.hpp"

class EquilateralTriangle: public CoreTriangle, public Object {
public:
    EquilateralTriangle(Shader *shader, const RenderData *data, Texture *texture, int reflection, Texture *reflectionMap, bool initLighting);
    ~EquilateralTriangle();
    
    void addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles);
    
    void render() = delete;
};

#endif /* equilateralTriangle_hpp */
