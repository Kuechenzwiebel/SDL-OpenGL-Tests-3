//
//  uiCoreTriangle.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 18.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef uiCoreTriangle_hpp
#define uiCoreTriangle_hpp

#include <stdio.h>

#include "../coreTriangle.hpp"
#include "../object.hpp"

class UICoreTriangle: public Object, public CoreTriangle {
public:
    UICoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, glm::vec2 *texOffset);
    
    void addToTriangleList(std::vector<CoreTriangle*> *triangles);
    
    void setTextureOffset(glm::vec2 offset);
    glm::vec2 getTextureOffset();
    
    void prepareRender();
    
private:
    Shader *shader;
    const RenderData *data;
    Texture *texture;
    
    glm::vec2 *offset;
    
    UniformVar<glm::vec2> textureOffset;
};

#endif /* uiCoreTriangle_hpp */
