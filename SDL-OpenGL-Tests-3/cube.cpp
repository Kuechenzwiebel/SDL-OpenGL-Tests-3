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

Cube::Cube(Shader *shader, const RenderData *data, Texture *texture, int reflection, Texture *reflectionMap):
shader(shader), data(data), texture(texture), reflectionMap(reflectionMap), reflection(reflection),
tris(shader, data, 12, cubeVertices, texture, cubeTexCoords, cubeNormals, &modelMat, reflection, reflectionMap, true) {
    
}

Cube::~Cube() {
    
}

void Cube::addToTriangleList(std::vector<CoreTriangleCluster*> *triangles) {
    triangles->push_back(&tris);
}
