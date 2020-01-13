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
#include "coreTriangle.hpp"

const static float sphereResolution = 5.0f;

class Sphere: public Object {
public:
    Sphere(Shader *shader, const RenderData *data, Texture *texture);
    ~Sphere();
    
    void addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles);
    
private:
    std::vector<std::unique_ptr<CoreTriangle>> tris;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
};

#endif /* sphere_hpp */
