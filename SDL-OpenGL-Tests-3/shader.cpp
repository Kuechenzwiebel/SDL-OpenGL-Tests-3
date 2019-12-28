//
//  shader.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "shader.hpp"

Shader::Shader(hg::File vertexFile, hg::File fragmentFile):
vertexFile(vertexFile), fragmentFile(fragmentFile) {
    compile(vertexFile.readFile().c_str(), fragmentFile.readFile().c_str());
}

Shader::~Shader() {
    glDeleteProgram(program);
}


void Shader::use() {
    if(program != 0) {
        glUseProgram(program);
    }
}

GLuint Shader::getProgram() {
    return program;
}

void Shader::compile(const char* vertexCode, const char* fragmentCode) {
    GLuint vertex = 0, fragment = 0;
    GLint success = 0;
    GLchar *infoLog = new GLchar[1];
    int errorLenght = 0;
    
    if(vertexCode == NULL) {
        printf("Vertexcode is NULL\n");
    }
    if(fragmentCode == NULL) {
        printf("Fragmentcode is NULL\n");
    }
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &errorLenght);
        infoLog = new GLchar[errorLenght];
        glGetShaderInfoLog(vertex, errorLenght, NULL, infoLog);
        
        printf(PRINTF_RED);
        printf("Vertex Shader failed compiliton! Infolog: %s\n", infoLog);
        printf(PRINTF_DEFAULT);
        exit(2);
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &errorLenght);
        infoLog = new GLchar[errorLenght];
        glGetShaderInfoLog(fragment, errorLenght, NULL, infoLog);
        
        printf(PRINTF_RED);
        printf("Fragment Shader failed compiliton! Infolog: %s\n", infoLog);
        printf(PRINTF_DEFAULT);
        exit(3);
    }
    
    
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    
    glGetShaderiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &errorLenght);
        infoLog = new GLchar[errorLenght];
        glGetShaderInfoLog(fragment, errorLenght, NULL, infoLog);
        
        printf(PRINTF_RED);
        printf("Shader linking failed! Infolog: %s\n", infoLog);
        printf(PRINTF_DEFAULT);
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    delete[] infoLog;
}
