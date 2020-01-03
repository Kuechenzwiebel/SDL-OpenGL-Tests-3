//
//  camera.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 03.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera(const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse);
    
    glm::mat4* getViewMatPointer();
    glm::vec3 getEyePosition();
    glm::vec3 getFootPosition();
    float getMouseSensitivity();
    float getZoom();
    
    void setEyePosition(glm::vec3 position);
    void setFootPosition(glm::vec3 position);
    void setMouseSensitivity(float sensitivity);
    void setZoom(float zoom);
    
    void processMouseInput();
    void processInput();
    
private:
    glm::mat4 viewMat;
    float yaw, pitch;
    glm::vec3 front, right, up;
    glm::vec3 footPosition, eyePosition;
    
    float mouseSensitivity;
    float zoom;
    
    const float *deltaTime;
    const SDL_Event *windowEvent;
    bool *checkMouse;
};

#endif /* camera_hpp */
