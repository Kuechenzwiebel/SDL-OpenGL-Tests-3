//
//  sphere.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 13.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef sphere_hpp
#define sphere_hpp

#include <stdio.h>
#include <vector>
#include <memory>

#include "object.hpp"
#include "coreTriangleCluster.hpp"

#define SPHERE_RESOLUTION 6.0f
#define SPHERE_ARRAY_SIZE (const int)((360.0f / SPHERE_RESOLUTION) * (360.0f / SPHERE_RESOLUTION) * 6 * 0.5f)

class Sphere: public Object {
public:
    Sphere(Shader *shader, const RenderData *data, Texture *texture, int reflection, Texture *reflectionMap);
    ~Sphere();
    
    void addToTriangleList(std::vector<CoreTriangleCluster*> *triangles);
    
private:
    std::unique_ptr<CoreTriangleCluster> tris;
    
    Texture *texture;
    Texture *reflectionMap;
    int reflection;
    int useReflectionMap;
    
    Shader *shader;
    const RenderData *data;
};

#endif /* sphere_hpp */
