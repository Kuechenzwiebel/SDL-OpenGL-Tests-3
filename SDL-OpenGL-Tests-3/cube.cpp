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
shader(shader), data(data), texture(texture),
tris({
    CoreTriangle(shader, data, &cubeVertices[0  * 3], texture, &cubeTexCoords[0  * 3], &cubeNormals[0  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[1  * 3], texture, &cubeTexCoords[1  * 3], &cubeNormals[1  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[2  * 3], texture, &cubeTexCoords[2  * 3], &cubeNormals[2  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[3  * 3], texture, &cubeTexCoords[3  * 3], &cubeNormals[3  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[4  * 3], texture, &cubeTexCoords[4  * 3], &cubeNormals[4  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[5  * 3], texture, &cubeTexCoords[5  * 3], &cubeNormals[5  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[6  * 3], texture, &cubeTexCoords[6  * 3], &cubeNormals[6  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[7  * 3], texture, &cubeTexCoords[7  * 3], &cubeNormals[7  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[8  * 3], texture, &cubeTexCoords[8  * 3], &cubeNormals[8  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[9  * 3], texture, &cubeTexCoords[9  * 3], &cubeNormals[9  * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[10 * 3], texture, &cubeTexCoords[10 * 3], &cubeNormals[10 * 3], &modelMat),
    CoreTriangle(shader, data, &cubeVertices[11 * 3], texture, &cubeTexCoords[11 * 3], &cubeNormals[11 * 3], &modelMat)}) {
    
}

Cube::~Cube() {
    
}

void Cube::addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles) {
    for(int i = 0; i < 12; i++) {
        triangles->push_back(std::make_pair(0.0f, &tris[i]));
    }
}
