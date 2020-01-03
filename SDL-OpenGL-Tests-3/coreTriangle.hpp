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
#include "texture.hpp"

class CoreTriangle: public Object {
public:
    CoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[]);
    ~CoreTriangle();
    
    glm::vec3 getMinVertex(glm::vec3 cameraPosition);
    
    void render();
    
    void addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles);
    
protected:
    GLuint VAO;
    ArrayBuffer<glm::vec3> vertex;
    ArrayBuffer<glm::vec2> texCoord;
    ArrayBuffer<glm::vec3> normal;
    
    const glm::vec3 *vertices;
    const glm::vec2 *uvs;
    const glm::vec3 *normals;
    Texture *texture;
    
    Shader *shader;
    const RenderData *data;
    
    UniformVar<glm::mat4> model;
    UniformVar<glm::mat4> view;
    UniformVar<glm::mat4> projection;
};

bool operator<(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r);
bool operator>(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r);

#endif /* coreTriangle_hpp */
