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

#include "coreTriangle.hpp"

class EquilateralTriangle: public CoreTriangle {
public:
    EquilateralTriangle(Shader *shader, const RenderData *data);
    ~EquilateralTriangle();
    
private:
    
};

#endif /* equilateralTriangle_hpp */
