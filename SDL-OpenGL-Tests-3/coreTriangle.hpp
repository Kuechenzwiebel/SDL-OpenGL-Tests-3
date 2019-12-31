//
//  coreTriangle.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef coreTriangle_hpp
#define coreTriangle_hpp

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

class CoreTriangle {
public:
    CoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[]);
    ~CoreTriangle();
    
    void setTranslation(glm::vec3 translation);
    void setRotation(glm::vec4 rotation);
    void setScale(glm::vec3 scale);
    void setModelMat(glm::mat4 model);
    
    glm::vec3 getTranslation();
    glm::vec4 getRotation();
    glm::vec3 getScale();
    glm::mat4 getModelMat();
    
    glm::vec3 getMaxVertex();
    
    virtual void prepareRender();
    void render();
    
private:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    
    const glm::vec3 *vertices;
    
    Shader *shader;
    const RenderData *data;
    
    UniformVar<glm::mat4> model;
    UniformVar<glm::mat4> view;
    UniformVar<glm::mat4> projection;
    
    glm::mat4 modelMat;
    
    virtual void calculateModelMat();
    
    glm::vec3 translation;
    glm::vec4 rotation;
    glm::vec3 scale;
};

bool operator<(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r);
bool operator>(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r);

#endif /* coreTriangle_hpp */
