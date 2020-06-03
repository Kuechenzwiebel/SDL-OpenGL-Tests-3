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

#include "object.hpp"

class CoreTriangle {
public:
    CoreTriangle(Shader *shader, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, int reflection, Texture *reflectionMap, bool initLighting);
    ~CoreTriangle();

    Shader* getShaderPointer();
    Texture* getTexturePointer();
    
    glm::vec3 getCenter();
    
    virtual void prepareRender();
    void render();
    
protected:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    ArrayBuffer<glm::vec3> normal;
    
    const glm::vec3 *vertices;
    const glm::vec2 *uvs;
    const glm::vec3 *normals;
    
    bool initLighting;
    
    Texture *texture;
    Texture *reflectionMap;
    int reflection;
    int useReflectionMap;
    
    Shader *shader;
    
    glm::mat4 *modelMatPointer;
    
    UniformVar<glm::mat4> model;
    
    std::unique_ptr<UniformVar<int>> reflectionUniform;
    std::unique_ptr<UniformVar<int>> useReflectionMapUniform;
};

bool operator<(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r);
bool operator>(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r);
bool operator==(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r);

#endif /* coreTriangle_hpp */
