//
//  spotLightSource.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 05.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef spotLightSource_hpp
#define spotLightSource_hpp

#include <stdio.h>

#include "lightSource.hpp"

void resetSpotLightSourceIndex();

class SpotLightSource: public LightSource {
public:
    SpotLightSource(Shader *shader);
    
    void activate();
    
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
    
private:
    Shader *shader;
    int idx;
    
    UniformVar<int> arrayIndexUniform;
    
    UniformVar<glm::vec3> positionUniform;
    UniformVar<glm::vec3> colorUniform;
    UniformVar<glm::vec3> directionUniform;
    UniformVar<float> cutOffUniform;
    UniformVar<float> outerCutOffUniform;
};

#endif /* spotLightSource_hpp */
