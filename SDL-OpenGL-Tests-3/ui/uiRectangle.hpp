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

class UIRectangle: public Object {
public:
    UIRectangle(Shader *shader, const RenderData *data, Texture *texture);
    
    void addToTriangleList(std::vector<CoreTriangle*> *triangles);
    
    void setModelMat(glm::mat4 model);
    
private:
    void calculateModelMat();
    
    CoreTriangle tri1, tri2;
    
    Texture *texture;
    Shader *shader;
    const RenderData *data;
};

#endif /* uiRectangle_hpp */
