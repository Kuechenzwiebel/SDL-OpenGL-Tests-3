//
//  ray.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 08.03.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef ray_hpp
#define ray_hpp

#include <stdio.h>

#include <glm/glm.hpp>

class Ray {
public:
    Ray() = default;
    
    void move(float distance);
    void recalculateDirection();
    
    glm::vec3 position, direction;
};

#endif /* ray_hpp */
