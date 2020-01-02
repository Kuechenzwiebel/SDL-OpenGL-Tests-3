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

#include <GL/glew.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"
#include "shader.hpp"
#include "uniformVar.hpp"
#include "arrayBuffers/arrayBuffer.hpp"

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
    
    virtual glm::vec3 getMaxVertex();
    
    virtual void prepareRender();
    virtual void render();
    
protected:
    glm::mat4 modelMat;
    
    virtual void calculateModelMat();
    
    glm::vec3 translation;
    glm::vec4 rotation;
    glm::vec3 scale;
};


bool operator<(std::pair<float, Object*> l, std::pair<float, Object*> r);
bool operator>(std::pair<float, Object*> l, std::pair<float, Object*> r);

#endif /* object_hpp */
