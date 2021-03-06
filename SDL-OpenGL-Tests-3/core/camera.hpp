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
#include <HG_Noise/HG_Noise.h>

#include <GL/glew.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../map/mapChunk.hpp"
#include "../utils.hpp"

class Camera {
public:
    Camera(const float *deltaTime, const SDL_Event *windowEvent, bool *checkMouse, hg::PerlinNoise *noise);

    glm::vec3* getEyePositionPointer();
    glm::vec3 getEyePosition();
    glm::vec3 getFootPosition();
    
    void setEyePosition(glm::vec3 position);
    void setFootPosition(glm::vec3 position);
    
    void processMouseInput();
    void preProcessInput();
    void processInput(MapDataVec3Type *mapVertices);
    
    
    glm::mat4 viewMat;
    
    float mouseSensitivity;
    float zoom;
    
    glm::vec3 front, right, up;
    float velocity;
    
    bool keyBoardControl;
    
private:
    float yaw, pitch;
    glm::vec3 footPosition, eyePosition;
    
    unsigned long timeSinceLastOnFloor;
    
    const float *deltaTime;
    const SDL_Event *windowEvent;
    bool *checkMouse;
    
    hg::PerlinNoise *noise;
};

#endif /* camera_hpp */
