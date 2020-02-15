//
//  texture.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 02.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#define GLEW_STATIC

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>

#include "shader.hpp"

class Texture {
public:
    Texture(std::string file, bool bitmap = true);
    Texture(unsigned char *data, unsigned int texWidth, unsigned int texHeight, bool bitmap = true);
    ~Texture();
    
    void activate(Shader *shader, int textureNumber = 0);
    
    GLuint textureID;
    std::string textureName;
    glm::vec2 textureSize;
    
    bool transparent;
};

#endif /* texture_hpp */
