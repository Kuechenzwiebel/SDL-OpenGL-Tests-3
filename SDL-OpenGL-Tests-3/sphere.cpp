//
//  sphere.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 13.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "sphere.hpp"

static glm::vec3 sphereSurface(float angleXZ, float angleXY) {
    glm::vec3 pos;
    pos.x = sin(glm::radians(angleXY)) * cos(glm::radians(angleXZ));
    pos.z = sin(glm::radians(angleXY)) * sin(glm::radians(angleXZ));
    pos.y = cos(glm::radians(angleXY));
    return pos;
}



Sphere::Sphere(Shader *shader, const RenderData *data, Texture *texture):
shader(shader), data(data), texture(texture) {
    glm::vec3 triangleVertices[2][3];
    glm::vec2 triangleUVs[2][3];
    
    for(float angleXY = 0.0f; angleXY < 180.0f; angleXY += sphereResolution) {
        for(float angleXZ = 0.0f; angleXZ < 360.0f; angleXZ += sphereResolution) {
            triangleVertices[0][0] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            triangleVertices[0][1] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            triangleVertices[0][2] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            
            triangleVertices[1][0] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            triangleVertices[1][1] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            triangleVertices[1][2] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            
            
            triangleUVs[0][0] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            triangleUVs[0][1] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            triangleUVs[0][2] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            
            triangleUVs[1][0] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            triangleUVs[1][1] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            triangleUVs[1][2] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            
            
            
            tris.push_back(std::make_unique<CoreTriangle>(shader, data, triangleVertices[0], texture, triangleUVs[0], triangleVertices[0], &modelMat));
            tris.push_back(std::make_unique<CoreTriangle>(shader, data, triangleVertices[1], texture, triangleUVs[1], triangleVertices[1], &modelMat));
        }
    }
}

Sphere::~Sphere() {
    
}

void Sphere::addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles) {
    for(int i = 0; i < tris.size(); i++) {
        triangles->push_back(std::make_pair(0.0f, tris[i].get()));
    }
}
