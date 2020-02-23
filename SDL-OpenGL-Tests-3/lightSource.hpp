//
//  lightSource.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 05.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef lightSource_hpp
#define lightSource_hpp

#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/shader.hpp"
#include "core/uniformVar.hpp"

class LightSource {
public:
    LightSource(Shader *shader);
    
    virtual void activate();
    
    void addToLightList(std::vector<LightSource*> *list);
    
    glm::vec3 position;
    glm::vec3 color;
    
protected:
    Shader *shader;
    int idx;
};

#endif /* lightSource_hpp */
