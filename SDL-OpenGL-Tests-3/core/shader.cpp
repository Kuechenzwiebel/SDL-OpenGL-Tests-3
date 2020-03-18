//
//  shader.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "shader.hpp"

Shader::Shader(hg::File vertexFile, hg::File fragmentFile)  {
    std::cout << "Shader " << vertexFile.getFileName() << " is being compiled" << std::endl;
    
    auto vertexLines = vertexFile.readFileLineByLine();
    auto fragmentLines = fragmentFile.readFileLineByLine();
    
    
    for(int i = 0; i < vertexLines.size(); i++) {
        if(hg::substr(*vertexLines[i], 0, 8) == "#include") {
            std::string includeFileName = hg::substr(*vertexLines[i], 10, (unsigned int)vertexLines[i]->size() - 1);
            
            hg::File includeFile(includeFileName);
            
            vertexLines.erase(vertexLines.begin() + i);
            vertexLines.insert(vertexLines.begin() + i, std::make_unique<std::string>(includeFile.readFile()));
        }
    }
    
    for(int i = 0; i < fragmentLines.size(); i++) {
        if(hg::substr(*fragmentLines[i], 0, 8) == "#include") {
            std::string includeFileName = hg::substr(*fragmentLines[i], 10, (unsigned int)fragmentLines[i]->size() - 1);
            
            hg::File includeFile(includeFileName);
            
            fragmentLines.erase(fragmentLines.begin() + i);
            fragmentLines.insert(fragmentLines.begin() + i, std::make_unique<std::string>(includeFile.readFile()));
        }
    }
    
    compile(hg::transformLinesToString(&vertexLines).c_str(), hg::transformLinesToString(&fragmentLines).c_str());
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
        printf("Vertex Shader failed compiliton!\nInfolog: \n%s\n", infoLog);
        printf(PRINTF_DEFAULT);
        
        printf("Source:\n%s", vertexCode);
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
        printf("Fragment Shader failed compiliton!\nInfolog: \n%s\n", infoLog);
        printf(PRINTF_DEFAULT);
        
        printf("Source:\n%s", fragmentCode);
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
        printf("Shader linking failed!\nInfolog: \n%s\n", infoLog);
        printf(PRINTF_DEFAULT);
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    delete[] infoLog;
}
