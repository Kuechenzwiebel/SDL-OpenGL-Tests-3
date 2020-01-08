//
//  cube.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 04.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "cube.hpp"

static glm::vec3 cubeVertices[] = {
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f,  0.5f, -0.5f),
    glm::vec3( 0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f, -0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3( 0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    
    glm::vec3(-0.5f,  0.5f, -0.5f),
    glm::vec3( 0.5f,  0.5f, -0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3( 0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f)
};

static glm::vec2 cubeTexCoords[] = {
    glm::vec2(0.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  0.0f),

    glm::vec2(0.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  0.0f),
    
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
 
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  1.0f),
    glm::vec2(0.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
   
    glm::vec2(0.0f,  1.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(0.0f,  0.0f),
    glm::vec2(0.0f,  1.0f),
   
    glm::vec2(0.0f,  1.0f),
    glm::vec2(1.0f,  1.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(1.0f,  0.0f),
    glm::vec2(0.0f,  0.0f),
    glm::vec2(0.0f,  1.0f)
};

static glm::vec3 cubeNormals[] = {
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f)
};

Cube::Cube(Shader *shader, const RenderData *data, Texture *texture):
shader(shader), data(data), texture(texture), ntris({CoreTriangle(shader, data, &cubeVertices[0 * 3], texture, &cubeTexCoords[0 * 3], &cubeNormals[0 * 3]), CoreTriangle(shader, data, &cubeVertices[0 * 3], texture, &cubeTexCoords[0 * 3], &cubeNormals[0 * 3])}) {
    for(int i = 0; i < 12; i++) {
        tris.push_back(std::make_unique<CoreTriangle>(shader, data, &cubeVertices[i * 3], texture, &cubeTexCoords[i * 3], &cubeNormals[i * 3]));
    }
}

Cube::~Cube() {
    
}

void Cube::setModelMat(glm::mat4 model) {
    modelMat = model;
    for(int i = 0; i < 12; i++) {
        tris[i]->setModelMat(modelMat);
    }
}

void Cube::addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles) {
    for(int i = 0; i < 12; i++) {
        tris[i]->addToTriangleList(triangles);
    }
}
