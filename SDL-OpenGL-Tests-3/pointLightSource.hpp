//
//  pointLightSource.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 25.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef pointLightSource_hpp
#define pointLightSource_hpp

#include <stdio.h>

#include <GL/glew.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "uniformVar.hpp"

void resetPointLightSourceIndex();

class PointLightSource {
public:
    PointLightSource(Shader *shader);
    
    void activate();
    
    glm::vec3 position;
    glm::vec3 color;
    float specularStrength;
    int reflection;
    
private:
    Shader *shader;
    int idx;
    
    UniformVar<int> arrayIndexUniform;
    UniformVar<glm::vec3> positionUniform;
    UniformVar<glm::vec3> colorUniform;
    UniformVar<float> specularStrengthUniform;
    UniformVar<int> reflectionUniform;
};

#endif /* pointLightSource_hpp */
