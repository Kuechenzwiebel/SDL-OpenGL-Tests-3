//
//  pointLightSource.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 25.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "pointLightSource.hpp"

static int arrayIndex = 0;

void resetPointLightSourceIndex() {
    arrayIndex = 0;
}

PointLightSource::PointLightSource(Shader *shader):
LightSource(shader), shader(shader),
positionUniform(shader, "pointLights[" + std::to_string(arrayIndex) + "].position", &position), colorUniform(shader, "pointLights[" + std::to_string(arrayIndex) + "].color", &color),
arrayIndexUniform(shader, "validPointLights", &idx) {
    idx = arrayIndex + 1;
    arrayIndex++;
}

void PointLightSource::activate() {
    arrayIndexUniform.setVar();
    
    positionUniform.setVar();
    colorUniform.setVar();
}
