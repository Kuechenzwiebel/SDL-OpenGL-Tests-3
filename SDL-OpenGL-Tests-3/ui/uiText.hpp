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

class UIText: public Object {
public:
    UIText(std::string text, Shader *shader, const RenderData *data);
    ~UIText();
    
    void setText(std::string newText);
    
    glm::vec2 getCharDimensions();
    
    void render();
    
private:
    std::vector<CoreTriangle*> triangles;
    std::vector<std::pair<std::pair<unsigned int, unsigned int>, std::unique_ptr<UIRectangle>>> chars;
    std::string text;
    
    void calculateModelMat();
    
    
    Shader *shader;
    const RenderData *data;
    
    Texture asciiBandTexture;
};

#endif /* uiText_hpp */
