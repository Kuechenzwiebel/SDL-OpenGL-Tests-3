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

using namespace glm;


int windowWidth = 1080, windowHeight = 760;
std::string windowTitle = "SDL-OpenGL-Tests-2";

bool running = true;

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
    
    SDL_Window *window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
    
    
    
    
    
    
    
    
    SDL_SetRelativeMouseMode(SDL_FALSE);
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
