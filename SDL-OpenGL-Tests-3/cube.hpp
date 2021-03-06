//
//  cube.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 04.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef cube_hpp
#define cube_hpp

#include <stdio.h>
#include <array>

#include "object.hpp"
#include "coreTriangle.hpp"
#include "coreTriangleCluster.hpp"
#include "coreTriangleElementCluster.hpp"

class Cube: public Object {
public:
    Cube(Shader *shader, Texture *texture, int reflection, Texture *reflectionMap);
    ~Cube();
    
    void addToTriangleList(std::vector<CoreTriangleCluster*> *triangles);
    
private:    
    CoreTriangleElementCluster<unsigned char> tris;
    
    Texture *texture;
    Texture *reflectionMap;
    int reflection;
    int useReflectionMap;
    
    Shader *shader;
    
    std::vector<CoreTriangleCluster*> *trianglePointer;
};

#endif /* cube_hpp */
