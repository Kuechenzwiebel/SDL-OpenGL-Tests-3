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
#include "coreTriangleArray.hpp"

const static float sphereResolution = 5.0f;
const static int sphereArraySize = round((360.0f / sphereResolution) * (360.0f / sphereResolution) * 6 * 0.5);

class Sphere: public Object {
public:
    Sphere(Shader *shader, const RenderData *data, Texture *texture);
    ~Sphere();
    
    void addToTriangleList(std::vector<CoreTriangleArray*> *triangles);
    
private:
    std::unique_ptr<CoreTriangleArray> tris;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
};

#endif /* sphere_hpp */
