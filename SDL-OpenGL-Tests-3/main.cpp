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
#include "coreTriangle.hpp"
#include "uniformVar.hpp"

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
    
    SDL_Window *window = SDL_CreateWindow("SDL-OpenGL-Tests-2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
    
    
    hg::File basicShaderVertex("resources/shader/basic.vs"), basicShaderFragment("resources/shader/basic.fs");
    Shader basicShader(basicShaderVertex, basicShaderFragment);
    
    RenderData renderData;
    
    
    CoreTriangle testTriangle(&basicShader, &renderData);
    UniformVar<glm::mat4> scale(&basicShader, "model");
    
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
            
            if(windowEvent.type == SDL_WINDOWEVENT) {
                if(windowEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                
                if(windowEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                    render = true;
                
                if(windowEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                    render = false;
            }
            
            if(windowEvent.type == SDL_MOUSEWHEEL) {
//                scale.setVar(scale.getVar() + vec2(float(windowEvent.wheel.x) * 0.15f, float(windowEvent.wheel.y) * 0.15f));
            }
            
            if(windowEvent.type == SDL_KEYDOWN) {
                if(windowEvent.key.keysym.sym == SDLK_ESCAPE) {
                    render = false;
                    checkMouse = false;
                }
            }
        }
        
        if(checkMouse)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
        
        if(render) {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glViewport(0, 0, windowWidth, windowHeight);
            
            
            basicShader.use();
            scale.setVar(glm::rotate(mat4(1), 3.14f / 2.0f, vec3(0.0f, 0.0f, 1.0f)));
            testTriangle.render();
            printf("Model\n");
            printMat4x4(glm::rotate(mat4(1), 3.14f / 2.0f, vec3(0.0f, 0.0f, 1.0f)));
            printf("Shader\n");
            printMat4x4(scale.getVar());
            
            
            SDL_GL_SwapWindow(window);
            glFlush();
            
            frame ++;
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
