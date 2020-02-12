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

glm::vec3 triangleVertices[SPHERE_ARRAY_SIZE] = {glm::vec3(0.0f)};
glm::vec2 triangleUVs[SPHERE_ARRAY_SIZE] = {glm::vec2(0.0f)};

static void initSphere() {
    int i = 0;
    
    for(float angleXY = 0.0f; angleXY < 180.0f; angleXY += SPHERE_RESOLUTION) {
        for(float angleXZ = 0.0f; angleXZ < 360.0f; angleXZ += SPHERE_RESOLUTION) {
            triangleVertices[i + 0] = sphereSurface(angleXZ + (0.0f * SPHERE_RESOLUTION), angleXY + (0.0f * SPHERE_RESOLUTION));
            triangleVertices[i + 1] = sphereSurface(angleXZ + (1.0f * SPHERE_RESOLUTION), angleXY + (0.0f * SPHERE_RESOLUTION));
            triangleVertices[i + 2] = sphereSurface(angleXZ + (0.0f * SPHERE_RESOLUTION), angleXY + (1.0f * SPHERE_RESOLUTION));
            
            triangleVertices[i + 3] = sphereSurface(angleXZ + (1.0f * SPHERE_RESOLUTION), angleXY + (0.0f * SPHERE_RESOLUTION));
            triangleVertices[i + 4] = sphereSurface(angleXZ + (0.0f * SPHERE_RESOLUTION), angleXY + (1.0f * SPHERE_RESOLUTION));
            triangleVertices[i + 5] = sphereSurface(angleXZ + (1.0f * SPHERE_RESOLUTION), angleXY + (1.0f * SPHERE_RESOLUTION));
            
            
            triangleUVs[i + 0] = glm::vec2(1.0f - (angleXZ +  (0.0f * SPHERE_RESOLUTION)) / 360.0f, 1.0f - (angleXY + (0.0f * SPHERE_RESOLUTION)) / 180.0f);
            triangleUVs[i + 1] = glm::vec2(1.0f - (angleXZ +  (1.0f * SPHERE_RESOLUTION)) / 360.0f, 1.0f - (angleXY + (0.0f * SPHERE_RESOLUTION)) / 180.0f);
            triangleUVs[i + 2] = glm::vec2(1.0f - (angleXZ +  (0.0f * SPHERE_RESOLUTION)) / 360.0f, 1.0f - (angleXY + (1.0f * SPHERE_RESOLUTION)) / 180.0f);
             
            triangleUVs[i + 3] = glm::vec2(1.0f - (angleXZ +  (1.0f * SPHERE_RESOLUTION)) / 360.0f, 1.0f - (angleXY + (0.0f * SPHERE_RESOLUTION)) / 180.0f);
            triangleUVs[i + 4] = glm::vec2(1.0f - (angleXZ +  (0.0f * SPHERE_RESOLUTION)) / 360.0f, 1.0f - (angleXY + (1.0f * SPHERE_RESOLUTION)) / 180.0f);
            triangleUVs[i + 5] = glm::vec2(1.0f - (angleXZ +  (1.0f * SPHERE_RESOLUTION)) / 360.0f, 1.0f - (angleXY + (1.0f * SPHERE_RESOLUTION)) / 180.0f);
            
            i += 6;
        }
    }
}

Sphere::Sphere(Shader *shader, const RenderData *data, Texture *texture, int reflection, Texture *reflectionMap):
shader(shader), data(data), texture(texture), reflectionMap(reflectionMap), reflection(reflection), trianglePointer(nullptr) {
    if(triangleVertices[0] == glm::vec3(0.0f)) {
        initSphere();
    }
    
    tris = std::make_unique<CoreTriangleCluster>(shader, data, SPHERE_ARRAY_SIZE / 3, triangleVertices, texture, triangleUVs, triangleVertices, &modelMat, reflection, reflectionMap, true);
}

Sphere::~Sphere() {
    if(trianglePointer != nullptr) {
        auto findIter = std::find(trianglePointer->begin(), trianglePointer->end(), tris.get());
        if(findIter != trianglePointer->end()) {
            trianglePointer->erase(findIter);
        }
    }
}

void Sphere::addToTriangleList(std::vector<CoreTriangleCluster*> *triangles) {
    triangles->push_back(tris.get());
    trianglePointer = triangles;
}
