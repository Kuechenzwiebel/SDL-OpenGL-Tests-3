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

glm::vec3 *triangleVertices = nullptr;
glm::vec2 *triangleUVs = nullptr;

static void initSphere() {
    triangleVertices = new glm::vec3[sphereArraySize];
    triangleUVs = new glm::vec2[sphereArraySize];
    
    int i = 0;
    
    for(float angleXY = 0.0f; angleXY < 180.0f; angleXY += sphereResolution) {
        for(float angleXZ = 0.0f; angleXZ < 360.0f; angleXZ += sphereResolution) {
            triangleVertices[i + 0] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            triangleVertices[i + 1] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            triangleVertices[i + 2] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            
            triangleVertices[i + 3] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (0.0f * sphereResolution));
            triangleVertices[i + 4] = sphereSurface(angleXZ + (0.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            triangleVertices[i + 5] = sphereSurface(angleXZ + (1.0f * sphereResolution), angleXY + (1.0f * sphereResolution));
            
            
            triangleUVs[i + 0] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            triangleUVs[i + 1] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            triangleUVs[i + 2] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            
            triangleUVs[i + 3] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (0.0f * sphereResolution)) / 180.0f);
            triangleUVs[i + 4] = glm::vec2(1.0f - (angleXZ + (0.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            triangleUVs[i + 5] = glm::vec2(1.0f - (angleXZ + (1.0f * sphereResolution)) / 360.0f, 1.0f - (angleXY + (1.0f * sphereResolution)) / 180.0f);
            
            i += 6;
        }
    }
}

static void deleteSphere() {
    delete [] triangleVertices;
    delete [] triangleUVs;
    
    triangleVertices = nullptr;
    triangleUVs = nullptr;
}

Sphere::Sphere(Shader *shader, const RenderData *data, Texture *texture):
shader(shader), data(data), texture(texture) {
    if(triangleVertices == nullptr) {
        initSphere();
    }
    
    tris = std::make_unique<CoreTriangleCluster>(shader, data, sphereArraySize / 3, triangleVertices, texture, triangleUVs, triangleVertices, &modelMat, 32, nullptr, true);
}

Sphere::~Sphere() {
    if(triangleVertices != nullptr) {
        deleteSphere();
    }
}

void Sphere::addToTriangleList(std::vector<CoreTriangleCluster*> *triangles) {
    triangles->push_back(tris.get());
}
