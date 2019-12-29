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
    UniformVar(Shader *shader, std::string varName, float *var);
    
    void getVar();
    void setVar();
    
private:
    Shader *shader;
    GLuint location;
    float *var;
};

template<>
class UniformVar<int> {
public:
    UniformVar(Shader *shader, std::string varName, int *var);
    
    void getVar();
    void setVar();
    
private:
    Shader *shader;
    GLuint location;
    int *var;
};


template<>
class UniformVar<glm::vec2> {
public:
    UniformVar(Shader *shader, std::string varName, glm::vec2 *var);
    
    void getVar();
    void setVar();
    
private:
    Shader *shader;
    GLuint location;
    glm::vec2 *var;
};

template<>
class UniformVar<glm::vec3> {
public:
    UniformVar(Shader *shader, std::string varName, glm::vec3 *var);
    
    void getVar();
    void setVar();
    
private:
    Shader *shader;
    GLuint location;
    glm::vec3 *var;
};

template<>
class UniformVar<glm::vec4> {
public:
    UniformVar(Shader *shader, std::string varName, glm::vec4 *var);
    
    void getVar();
    void setVar();
    
private:
    Shader *shader;
    GLuint location;
    glm::vec4 *var;
};


template<>
class UniformVar<glm::mat4> {
public:
    UniformVar(Shader *shader, std::string varName, glm::mat4 *var);
    
    void getVar();
    void setVar();
    
private:
    Shader *shader;
    GLuint location;
    glm::mat4 *var;
};

#endif /* uniformVar_hpp */
