//
//  spotLightSource.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 05.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "spotLightSource.hpp"

static int arrayIndex = 0;

void resetSpotLightSourceIndex() {
    arrayIndex = 0;
}

SpotLightSource::SpotLightSource(Shader *shader):
LightSource(shader), shader(shader), direction(1.0f), cutOff(glm::cos(glm::radians(5.0f))), outerCutOff(glm::cos(glm::radians(6.0f))),
positionUniform(shader, "spotLights[" + std::to_string(arrayIndex) + "].position", &position), colorUniform(shader, "spotLights[" + std::to_string(arrayIndex) + "].color", &color), directionUniform(shader, "spotLights[" + std::to_string(arrayIndex) + "].direction", &direction), cutOffUniform(shader, "spotLights[" + std::to_string(arrayIndex) + "].cutOff", &cutOff), outerCutOffUniform(shader, "spotLights[" + std::to_string(arrayIndex) + "].outerCutOff", &outerCutOff),
arrayIndexUniform(shader, "validSpotLights", &idx) {
    idx = arrayIndex + 1;
    arrayIndex++;
}

void SpotLightSource::activate() {
    arrayIndexUniform.setVar();
    
    positionUniform.setVar();
    colorUniform.setVar();
    directionUniform.setVar();
    cutOffUniform.setVar();
    outerCutOffUniform.setVar();
}
