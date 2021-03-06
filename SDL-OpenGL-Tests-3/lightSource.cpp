//
//  lightSource.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 05.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "lightSource.hpp"

LightSource::LightSource(Shader *shader):
shader(shader), position(0.0f), color(1.0f) {
    
}

void LightSource::activate() {
    
}

void LightSource::addToLightList(std::vector<LightSource*> *list) {
    list->push_back(this);
}
