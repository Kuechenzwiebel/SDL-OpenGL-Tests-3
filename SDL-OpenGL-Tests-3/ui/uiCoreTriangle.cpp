//
//  uiCoreTriangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 18.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "uiCoreTriangle.hpp"

UICoreTriangle::UICoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[], glm::mat4 *modelMat, glm::vec2 *texOffset):
CoreTriangle(shader, data, vertices, texture, uvs, normals, modelMat),
textureOffset(shader, "texOffset", texOffset), offset(texOffset) {
    
}

void UICoreTriangle::prepareRender() {
    textureOffset.setVar();
}

void UICoreTriangle::addToTriangleList(std::vector<CoreTriangle*> *triangles) {
    triangles->push_back(this);
}
