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
shader(shader), specularStrength(1.0f), position(0.0f), color(1.0f), reflection(64),
positionUniform(shader, "lights[" + std::to_string(arrayIndex) + "].lightPos", &position), colorUniform(shader, "lights[" + std::to_string(arrayIndex) + "].lightColor", &color), arrayIndexUniform(shader, "validObjects", &idx) {
    idx = arrayIndex + 1;
    arrayIndex++;
}

void PointLightSource::activate() {
    arrayIndexUniform.setVar();
    
    positionUniform.setVar();
    colorUniform.setVar();
}
