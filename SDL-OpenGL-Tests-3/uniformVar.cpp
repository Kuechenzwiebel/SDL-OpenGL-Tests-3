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


UniformVar<float>::UniformVar(Shader *shader, std::string varName):
shader(shader) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

float UniformVar<float>::getVar() {
    float f;
    glGetUniformfv(shader->getProgram(), location, &f);
    return f;
}

void UniformVar<float>::setVar(float var) {
    glUniform1f(location, var);
}


UniformVar<int>::UniformVar(Shader *shader, std::string varName):
shader(shader) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

int UniformVar<int>::getVar() {
    int f;
    glGetUniformiv(shader->getProgram(), location, &f);
    return f;
}

void UniformVar<int>::setVar(int var) {
    glUniform1i(location, var);
}



UniformVar<glm::vec2>::UniformVar(Shader *shader, std::string varName):
shader(shader) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

glm::vec2 UniformVar<glm::vec2>::getVar() {
    float f[2];
    glGetUniformfv(shader->getProgram(), location, &f[0]);
    return glm::vec2(f[0], f[1]);
}

void UniformVar<glm::vec2>::setVar(glm::vec2 var) {
    glUniform2f(location, var.x, var.y);
}


UniformVar<glm::vec3>::UniformVar(Shader *shader, std::string varName):
shader(shader) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

glm::vec3 UniformVar<glm::vec3>::getVar() {
    float f[3];
    glGetUniformfv(shader->getProgram(), location, &f[0]);
    return glm::vec3(f[0], f[1], f[2]);
}

void UniformVar<glm::vec3>::setVar(glm::vec3 var) {
    glUniform3f(location, var.x, var.y, var.z);
}


UniformVar<glm::vec4>::UniformVar(Shader *shader, std::string varName):
shader(shader) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

glm::vec4 UniformVar<glm::vec4>::getVar() {
    float f[4];
    glGetUniformfv(shader->getProgram(), location, &f[0]);
    return glm::vec4(f[0], f[1], f[2], f[3]);
}

void UniformVar<glm::vec4>::setVar(glm::vec4 var) {
    glUniform4f(location, var.x, var.y, var.z, var.w);
}



UniformVar<glm::mat4>::UniformVar(Shader *shader, std::string varName):
shader(shader) {
    location = getLoc(shader->getProgram(), varName);
    
    if(location == -1) {
        printf(PRINTF_RED);
        printf("Variable %s was not found!\n", varName.c_str());
        printf(PRINTF_DEFAULT);
        exit(1);
    }
}

glm::mat4 UniformVar<glm::mat4>::getVar() {
    glm::mat4 mat(1);
    glGetUniformfv(shader->getProgram(), location, &mat[0][0]);
    return mat;
}

void UniformVar<glm::mat4>::setVar(glm::mat4 var) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(var));
}

