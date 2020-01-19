//
//  uiText.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 18.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef uiText_hpp
#define uiText_hpp

#include <stdio.h>
#include <vector>
#include <memory>
#include <string>

#include <HG_List/HG_List.h>

#include "uiRectangle.hpp"

extern float mouseWheel;

class UIText: public Object {
public:
    UIText(std::string text, Shader *shader, const RenderData *data);
    ~UIText();
    
    void render();
    
private:
    std::vector<std::unique_ptr<UIRectangle>> chars;
    
    void calculateModelMat();
    
    Shader *shader;
    const RenderData *data;
    
    std::vector<CoreTriangle*> triangles;
    Texture asciiBandTexture;
};

#endif /* uiText_hpp */
