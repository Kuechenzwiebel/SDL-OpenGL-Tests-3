//
//  object.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 02.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef object_hpp
#define object_hpp

#include <stdio.h>
#include <list>
#include <utility>

#include <GL/glew.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"
#include "core/shader.hpp"
#include "core/texture.hpp"
#include "core/uniformVar.hpp"
#include "core/arrayBuffers/arrayBuffer.hpp"

class CoreTriangle;

class Object {
protected:
    Object();
    
public:
    virtual ~Object();
    
    virtual void setTranslation(glm::vec3 translation);
    virtual void setRotation(glm::vec4 rotation);
    virtual void setScale(glm::vec3 scale);
    virtual void setModelMat(glm::mat4 model);
    
    virtual glm::vec3 getTranslation();
    virtual glm::vec4 getRotation();
    virtual glm::vec3 getScale();
    virtual glm::mat4 getModelMat();
    
protected:
    glm::mat4 modelMat;
    
    virtual void calculateModelMat();
    
    glm::vec3 translation;
    glm::vec4 rotation;
    glm::vec3 scale;
};

#endif /* object_hpp */
