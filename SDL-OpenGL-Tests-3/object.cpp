//
//  object.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 02.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "object.hpp"

Object::Object():
translation(0.0f), scale(1.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), modelMat(1) {
    
}

Object::~Object() {
    
}

void Object::addToTriangleList(std::list<std::pair<float, CoreTriangle*>> *triangles) {
    
}

void Object::setTranslation(glm::vec3 translation) {
    this->translation = translation;
    calculateModelMat();
}

void Object::setRotation(glm::vec4 rotation) {
    this->rotation = rotation;
    calculateModelMat();
}

void Object::setScale(glm::vec3 scale) {
    this->scale = scale;
    calculateModelMat();
}
   
void Object::setModelMat(glm::mat4 model) {
    modelMat = model;
}
   
glm::vec3 Object::getTranslation() {
    return translation;
}

glm::vec4 Object::getRotation() {
    return rotation;
}

glm::vec3 Object::getScale() {
    return scale;
}

glm::mat4 Object::getModelMat() {
    return modelMat;
}

void Object::calculateModelMat() {
    modelMat = glm::translate(glm::mat4(1), translation) * glm::rotate(glm::mat4(1), rotation.w, rotation.xyz()) * glm::scale(glm::mat4(1), scale);
}

