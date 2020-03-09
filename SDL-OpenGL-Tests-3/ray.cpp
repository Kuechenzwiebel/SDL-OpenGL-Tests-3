//
//  ray.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 08.03.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "ray.hpp"

void Ray::move(float distance) {
    position = position + direction * distance;
}

void Ray::recalculateDirection() {
    direction = glm::normalize(direction - position);
}
