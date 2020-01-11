//
//  uiRectangle.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 11.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef uiRectangle_hpp
#define uiRectangle_hpp

#include <stdio.h>
#include <vector>

#include "../coreTriangle.hpp"
#include "../object.hpp"

class UIRectangle: public Object {
public:
    UIRectangle(Shader *shader, const RenderData *data, Texture *texture);
    
    void addToTriangleList(std::vector<CoreTriangle*> *triangles);
    
private:
    CoreTriangle tri1, tri2;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
};

#endif /* uiRectangle_hpp */
