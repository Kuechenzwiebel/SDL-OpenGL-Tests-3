//
//  coreTriangle.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "coreTriangle.hpp"

CoreTriangle::CoreTriangle(Shader *shader, const RenderData *data, const glm::vec3 vertices[], Texture *texture, const glm::vec2 uvs[], const glm::vec3 normals[]):
vertices(vertices), uvs(uvs), normals(normals), shader(shader), data(data),
vertex(vertices, sizeof(glm::vec3) * 3, 0), texCoord(uvs, sizeof(glm::vec2) * 3, 1), normal(normals, sizeof(glm::vec3) * 3, 2),
model(shader, "model", &modelMat), view(shader, "view", data->viewMat), projection(shader, "projection", data->projection), texture(texture) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    glBindVertexArray(0);
}

CoreTriangle::~CoreTriangle() {
    glDeleteVertexArrays(1, &VAO);
}

void CoreTriangle::render() {
    vertex.activate();
    texCoord.activate();
    normal.activate();
    
    projection.setVar();
    view.setVar();
    model.setVar();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void CoreTriangle::addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles) {
    triangles->push_back(std::make_pair(0.0f, this));
}

glm::vec3 CoreTriangle::getMinVertex(glm::vec3 cameraPosition) {
    int i = 0;
    
    float distance1 = glm::distance(cameraPosition, (modelMat * glm::vec4(vertices[0], 1.0f)).xyz());
    float distance2 = glm::distance(cameraPosition, (modelMat * glm::vec4(vertices[1], 1.0f)).xyz());
    float distance3 = glm::distance(cameraPosition, (modelMat * glm::vec4(vertices[2], 1.0f)).xyz());
    
    if(distance1 < distance2 && distance1 < distance3) {
        i = 0;
    }
    else if(distance2 < distance1 && distance2 < distance3) {
        i = 1;
    }
    else if(distance3 < distance1 && distance3 < distance2) {
        i = 2;
    }
    
    return (modelMat * glm::vec4(vertices[i], 1.0f)).xyz();
}



bool operator<(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r) {
    if(l.first < r.first) {
        return true;
    }
    else {
        return false;
    }
}

bool operator>(std::pair<float, CoreTriangle*> l, std::pair<float, CoreTriangle*> r) {
    if(l.first > r.first) {
        return true;
    }
    else {
        return false;
    }
}
