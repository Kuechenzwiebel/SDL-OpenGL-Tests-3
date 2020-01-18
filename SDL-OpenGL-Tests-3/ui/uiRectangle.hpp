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

#include "uiCoreTriangle.hpp"
#include "../coreTriangle.hpp"
#include "../object.hpp"

class UIRectangle: public Object {
public:
    UIRectangle(Shader *shader, const RenderData *data, Texture *texture);
    UIRectangle(Shader *shader, const RenderData *data, Texture *texture, const glm::vec2 *customUVs[]);
    
    void addToTriangleList(std::vector<CoreTriangle*> *triangles);
    
    void setTextureOffset(glm::vec2 offset);
    glm::vec2 getTextureOffset();
    
private:
    UICoreTriangle tri1, tri2;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
    
    glm::vec2 offset;
};

#endif /* uiRectangle_hpp */
