//
//  main.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>
#include <list>
#include <algorithm>
#include <utility>
#include <chrono>
#include <thread>
#include <mutex>
#include <iomanip>

#define GLEW_STATIC
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SOIL2/SOIL2.h>

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>

#include "utils.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "coreTriangle.hpp"
#include "uniformVar.hpp"
#include "equilateralTriangle.hpp"
#include "cube.hpp"
#include "ui/uiRectangle.hpp"

using namespace glm;


int windowWidth = 1080, windowHeight = 760;

bool running = true;
bool render = true;
bool checkMouse = false;

int main(int argc, const char * argv[]) {
    std::cout << "Main Thread ID: " << std::this_thread::get_id() << std::endl;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL2");
        return EXIT_FAILURE;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    SDL_Window *window = SDL_CreateWindow("SDL-OpenGL-Tests-3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_Event windowEvent;
    
    SDL_GL_SetSwapInterval(1);
    
    if (window == NULL) {
        printf(PRINTF_RED);
        printf("Failed to open Window!\n");
        printf(PRINTF_DEFAULT);
        return EXIT_FAILURE;
    }
    
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK) {
        printf(PRINTF_RED);
        printf("Failed to initialize GLEW!\n");
        printf(PRINTF_DEFAULT);
        return EXIT_FAILURE;
    }
    
    glViewport(0, 0, windowWidth, windowHeight);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    
    
    int frame = 0;
    long nextMeasure = SDL_GetTicks() + 1e3;
    int fps = 0;
    unsigned long long totalFrames = 0;
    
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float deltaTime = 0.0f;
    
    std::list<std::pair<float, CoreTriangle*>> triangles;
    std::vector<CoreTriangle*> uiTriangles;
    
    hg::File basicShaderVertex("resources/shader/basic.vs"), basicShaderFragment("resources/shader/basic.fs");
    Shader basicShader(basicShaderVertex, basicShaderFragment);
    
    
    
    Camera cam(&deltaTime, &windowEvent, &checkMouse);
    mat4 projectionMat = infinitePerspective(radians(cam.getZoom()), float(windowWidth) / float(windowHeight), 0.005f);
    
    mat4 uiProjection = ortho(-0.5f * windowWidth, 0.5f * windowWidth, -0.5f * windowHeight, 0.5f * windowHeight, -1.0f, 1.0f);
    mat4 uiView = lookAt(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    
    RenderData renderData;
    renderData.projection = &projectionMat;
    renderData.viewMat = cam.getViewMatPointer();
    
    RenderData uiData;
    uiData.projection = &uiProjection;
    uiData.viewMat = &uiView;
    
    
    Texture debugTexture("resources/texture/debug.png");
    Texture debug2Texture("resources/texture/debug2.png");
    
    unsigned char data[] = {
        255,
        255,
        255,
        100};
    
    unsigned char data2[] = {
           0,
           0,
           0,
           0xff};
    
    Texture transparentTexture(data, 1, 1, false);
    Texture blackTexture(data2, 1, 1, false);
    
    std::vector<std::unique_ptr<CoreTriangle>> tris;
    
    glm::vec3 triangleVertices[] = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(-1.0f,  1.0f, 0.0f)
    };
    
    glm::vec2 triangleUVs[] = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)
    };
    
    glm::vec3 triangleNormals[] = {
        glm::triangleNormal(triangleVertices[0], triangleVertices[1], triangleVertices[2]),
        glm::triangleNormal(triangleVertices[0], triangleVertices[1], triangleVertices[2]),
        glm::triangleNormal(triangleVertices[0], triangleVertices[1], triangleVertices[2])
    };
    
    
    CoreTriangle uiTriangle(&basicShader, &uiData, triangleVertices, &debug2Texture, triangleUVs, triangleNormals);
//    uiTriangles.push_back(&uiTriangle);
    
    UIRectangle uiRect(&basicShader, &uiData, &debug2Texture);
    uiRect.addToTriangleList(&uiTriangles);
    
    
    for(int i = 0; i < 20; i++) {
        tris.push_back(std::make_unique<CoreTriangle>(&basicShader, &renderData, triangleVertices, &transparentTexture, triangleUVs, triangleNormals));
        tris[i]->addToTriangleList(&triangles);
        tris[i]->setTranslation(vec3(float(i / 3.0f) + 2.0f, 0.0f, 0.0f));
        tris[i]->setRotation(vec4(0.0f, 1.0f, 0.0f, HALF_PI));
    }
    
    EquilateralTriangle e(&basicShader, &renderData, &transparentTexture);
    e.setTranslation(vec3(0.0f, 0.0f, -2.0f));
    e.setRotation(vec4(0.0f, 1.0f, 0.0f, HALF_PI));
    e.addToTriangleList(&triangles);
    
    Cube cube(&basicShader, &renderData, &debug2Texture);
    cube.setModelMat(translate(mat4(1), vec3(1.0f)));
    cube.addToTriangleList(&triangles);
    
    float mouseWheel = 0.0f;
    
    printf("%lu of %E possible triangles registerd\n", triangles.size(), double(triangles.max_size()));

    while(running) {
        if(SDL_GetTicks() > nextMeasure) {
            fps = frame;
            frame = 0;
            nextMeasure += 1000;
        }
        
        currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        while(SDL_PollEvent(&windowEvent) != 0) {
            if(windowEvent.type == SDL_QUIT)
                running = false;
            
            if(windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                if(windowEvent.button.button == SDL_BUTTON_LEFT) {
                    checkMouse = true;
                    render = true;
                }
            }
            
            cam.processMouseInput();
            
            if(windowEvent.type == SDL_WINDOWEVENT) {
                if(windowEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                
                if(windowEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                    render = true;
                
                if(windowEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                    render = false;
            }
            
            if(windowEvent.type == SDL_KEYDOWN) {
                if(windowEvent.key.keysym.sym == SDLK_ESCAPE) {
                    render = false;
                    checkMouse = false;
                }
            }
            
            if(windowEvent.type == SDL_MOUSEWHEEL) {
                mouseWheel += float(windowEvent.wheel.y) * 0.15f;
            }
        }
        
        if(checkMouse)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
        
        if(render) {
            cam.processInput();
            projectionMat = infinitePerspective(radians(cam.getZoom()), float(windowWidth) / float(windowHeight), 0.005f);
            uiProjection = ortho(-0.5f * float(windowWidth), 0.5f * float(windowWidth), -0.5f * float(windowHeight), 0.5f * float(windowHeight), -1.0f, 1.0f);
            
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glViewport(0, 0, windowWidth, windowHeight);
            
            cube.setModelMat(rotate(mat4(1), SDL_GetTicks() / 1000.0f, vec3(1.0f)));
            
            uiRect.setScale(vec3(500.0f));
            uiRect.setTranslation(vec3(float(windowWidth) / 2.0f - 250.0f, -float(windowHeight) / 2.0f + 250.0f, 0.0f));
            uiRect.setRotation(vec4(0.0f, 0.0f, 1.0f, mouseWheel));

            for(auto it = triangles.begin(); it != triangles.end(); it++) {
                it->first = glm::distance(cam.getEyePosition(), it->second->getCenter());
            }
            
            triangles.sort();
            
            basicShader.use();
            
            for(auto it = triangles.rbegin(); it != triangles.rend(); it++) {
                it->second->render();
            }
            
            glClear(GL_DEPTH_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
            for(int i = 0; i < uiTriangles.size(); i++) {
                uiTriangles[i]->render();
            }
            
            SDL_GL_SwapWindow(window);
            glFlush();
            
            std::cout << mouseWheel << std::endl;
            
            frame++;
            totalFrames++;
        }
        else
            SDL_Delay(33);
    }
    
    SDL_SetRelativeMouseMode(SDL_FALSE);
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
