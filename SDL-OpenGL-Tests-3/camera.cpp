//
//  camera.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 03.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "camera.hpp"

Camera::Camera(const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse):
deltaTime(deltaTime), windowEvent(windowEvent), checkMouse(checkMouse), viewMat(1), mouseSensitivity(0.25f), zoom(45.0f), front(0.0f, 0.0f, -1.0f), right(1.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f), footPosition(0.0f), eyePosition(footPosition + glm::vec3(0.0f, 1.73f, 0.0f)) {
    
}

glm::mat4* Camera::getViewMatPointer() {
    return &viewMat;
}

void Camera::processMouseInput() {
    if(*checkMouse) {
        if(windowEvent->type == SDL_MOUSEMOTION) {
            this->yaw += windowEvent->motion.xrel * this->mouseSensitivity;
            this->pitch -= windowEvent->motion.yrel * this->mouseSensitivity;
            
            if (this->pitch >= 90.0f) {
                this->pitch = 89.99f;
            }
            
            if (this->pitch <= -90.0f) {
                this->pitch = -89.99f;
            }
            
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            
            front = glm::normalize(front);
            right = glm::normalize(cross(front, up));
        }
    }
    
    yaw = fmod(yaw + 360.0f, 360.0f);
}

void Camera::processInput() {
    if(*checkMouse) {
        const Uint8 *keystates = SDL_GetKeyboardState(NULL);
        
        glm::vec3 movementVector(0.0f);
        
        if(keystates[SDL_SCANCODE_W])
            movementVector += this->front;
        if(keystates[SDL_SCANCODE_D])
            movementVector += this->right;
        if(keystates[SDL_SCANCODE_S])
            movementVector -= this->front;
        if(keystates[SDL_SCANCODE_A])
            movementVector -= this->right;
        if(keystates[SDL_SCANCODE_SPACE])
            movementVector += glm::vec3(0.0f, 1.0f, 0.0f);
        if(keystates[SDL_SCANCODE_LSHIFT])
            movementVector -= glm::vec3(0.0f, 1.0f, 0.0f);
        
        if(movementVector != glm::vec3(0.0f))
            footPosition += glm::normalize(movementVector) * *deltaTime * 1.5f;
    }
    
    eyePosition = footPosition + glm::vec3(0.0f, 1.73f, 0.0f);
    
    
    viewMat = glm::lookAt(this->eyePosition, this->eyePosition + this->front, this->up);
}



glm::vec3 Camera::getEyePosition() {
    return eyePosition;
}

glm::vec3 Camera::getFootPosition() {
    return footPosition;
}

float Camera::getMouseSensitivity() {
    return mouseSensitivity;
}

float Camera::getZoom() {
    return zoom;
}


void Camera::setEyePosition(glm::vec3 position) {
    eyePosition = position;
    footPosition = eyePosition - glm::vec3(0.0f, 1.73f, 0.0f);
}

void Camera::setFootPosition(glm::vec3 position) {
    footPosition = position;
    eyePosition = footPosition + glm::vec3(0.0f, 1.73f, 0.0f);
}

void Camera::setMouseSensitivity(float sensitivity) {
    mouseSensitivity = sensitivity;
}

void Camera::setZoom(float zoom) {
    this->zoom = zoom;
}
