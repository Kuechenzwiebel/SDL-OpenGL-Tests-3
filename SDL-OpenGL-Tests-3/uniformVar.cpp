//
//  uniformVar.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "uniformVar.hpp"

static GLint getLoc(GLuint program, std::string name) {
    return glGetUniformLocation(program, name.c_str());
}


UniformVar<float>::UniformVar(Shader *shader, std::string varName, float *var):
shader(shader), var(var) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

void UniformVar<float>::getVar() {
    glGetUniformfv(shader->getProgram(), location, var);
}

void UniformVar<float>::setVar() {
    glUniform1f(location, *var);
}


UniformVar<int>::UniformVar(Shader *shader, std::string varName, int *var):
shader(shader), var(var) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

void UniformVar<int>::getVar() {
    glGetUniformiv(shader->getProgram(), location, var);
}

void UniformVar<int>::setVar() {
    glUniform1i(location, *var);
}



UniformVar<glm::vec2>::UniformVar(Shader *shader, std::string varName, glm::vec2 *var):
shader(shader), var(var) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

void UniformVar<glm::vec2>::getVar() {
    glGetUniformfv(shader->getProgram(), location, &var->x);
}

void UniformVar<glm::vec2>::setVar() {
    glUniform2f(location, var->x, var->y);
}


UniformVar<glm::vec3>::UniformVar(Shader *shader, std::string varName, glm::vec3 *var):
shader(shader), var(var) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

void UniformVar<glm::vec3>::getVar() {
    glGetUniformfv(shader->getProgram(), location, &var->x);
}

void UniformVar<glm::vec3>::setVar() {
    glUniform3f(location, var->x, var->y, var->z);
}


UniformVar<glm::vec4>::UniformVar(Shader *shader, std::string varName, glm::vec4 *var):
shader(shader), var(var) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

void UniformVar<glm::vec4>::getVar() {
    glGetUniformfv(shader->getProgram(), location, &var->x);
}

void UniformVar<glm::vec4>::setVar() {
    glUniform4f(location, var->x, var->y, var->z, var->w);
}



UniformVar<glm::mat4>::UniformVar(Shader *shader, std::string varName, glm::mat4 *var):
shader(shader), var(var) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

void UniformVar<glm::mat4>::getVar() {
    glGetUniformfv(shader->getProgram(), location, &(*var)[0][0]);
}

void UniformVar<glm::mat4>::setVar() {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(*var));
}

