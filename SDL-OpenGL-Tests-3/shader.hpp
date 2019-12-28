//
//  shader.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>
#include <string>

#include <HG_List/HG_List.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"

class Shader {
public:
    Shader(hg::File vertexFile, hg::File fragmentFile);
    ~Shader();
    
    GLuint getProgram();
    
    void use();
    
private:
    hg::File vertexFile;
    hg::File fragmentFile;
    
    void compile(const char* vertexCode, const char* fragmentCode);
    GLuint program;
};

#endif /* shader_hpp */
