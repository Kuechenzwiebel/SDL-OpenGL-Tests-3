//
//  uniformVar.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef uniformVar_hpp
#define uniformVar_hpp

#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "utils.hpp"

template <typename T>
class UniformVar {
public:
    UniformVar() = delete;
};


template<>
class UniformVar<float> {
public:
    UniformVar(Shader *shader, std::string varName);
    
    float getVar();
    void setVar(float var);
    
private:
    Shader *shader;
    GLuint location;
};

template<>
class UniformVar<int> {
public:
    UniformVar(Shader *shader, std::string varName);
    
    int getVar();
    void setVar(int var);
    
private:
    Shader *shader;
    GLuint location;
};


template<>
class UniformVar<glm::vec2> {
public:
    UniformVar(Shader *shader, std::string varName);
    
    glm::vec2 getVar();
    void setVar(glm::vec2 var);
    
private:
    Shader *shader;
    GLuint location;
};

template<>
class UniformVar<glm::vec3> {
public:
    UniformVar(Shader *shader, std::string varName);
    
    glm::vec3 getVar();
    void setVar(glm::vec3 var);
    
private:
    Shader *shader;
    GLuint location;
};

template<>
class UniformVar<glm::vec4> {
public:
    UniformVar(Shader *shader, std::string varName);
    
    glm::vec4 getVar();
    void setVar(glm::vec4 var);
    
private:
    Shader *shader;
    GLuint location;
};


template<>
class UniformVar<glm::mat4> {
public:
    UniformVar(Shader *shader, std::string varName);
    
    glm::mat4 getVar();
    void setVar(glm::mat4 var);
    
private:
    Shader *shader;
    GLuint location;
};

#endif /* uniformVar_hpp */
