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
#include <atomic>
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
#include "sphere.hpp"
#include "coreTriangleCluster.hpp"
#include "objModel.hpp"
#include "ui/uiText.hpp"
#include "pointLightSource.hpp"

using namespace glm;


int windowWidth = 1080, windowHeight = 760;

bool running = true;
bool render = true;
bool wireframe = false;
bool checkMouse = false;


float mouseWheel = 0.0f;


std::thread sortThread;
std::atomic<bool> sortDone;
std::atomic<bool> sort;

glm::vec3 oldCamFootPos;

void sortTriangles(Camera *cam, std::list<std::pair<float, CoreTriangle*>> *transparentTriangles) {
    std::cout << "Sort Thread ID: " << std::this_thread::get_id() << std::endl;
    
    while(running) {
        if(sort) {
            sortDone = false;
            
            if(cam->getFootPosition() != oldCamFootPos) {
                for(auto it = transparentTriangles->begin(); it != transparentTriangles->end(); it++) {
                    it->first = glm::distance(cam->getEyePosition(), it->second->getCenter());
                }
                
                transparentTriangles->sort();
            }
            
            sortDone = true;
            sort = false;
        }
        else {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }
}


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
    unsigned int fps = 0;
    unsigned long long totalFrames = 0;
    
    unsigned long fpsSum = 0;
    unsigned int measures = 0;
    
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
    
    std::list<std::pair<float, CoreTriangle*>> transparentTriangles;
    std::vector<CoreTriangleCluster*> opaqueTriangles;
    
    std::vector<CoreTriangle*> uiTriangles;
    std::vector<UIText*> uiTexts;
    
    hg::File basicShaderVertex("resources/shader/basic.vs"), basicShaderFragment("resources/shader/basic.fs");
    hg::File uiShaderVertex("resources/shader/ui.vs"), uiShaderFragment("resources/shader/ui.fs");
    Shader basicShader(basicShaderVertex, basicShaderFragment);
    Shader uiShader(uiShaderVertex, uiShaderFragment);
    
    
    
    Camera cam(&deltaTime, &windowEvent, &checkMouse);
    UniformVar<vec3> viewPos(&basicShader, "viewPos", cam.getEyePositionPointer());
    cam.processMouseInput();
    cam.processInput();
    
    mat4 projectionMat = infinitePerspective(radians(cam.getZoom()), float(windowWidth) / float(windowHeight), 0.005f);
    mat4 uiProjection = ortho(-0.5f * windowWidth, 0.5f * windowWidth, -0.5f * windowHeight, 0.5f * windowHeight, -1.0f, 1.0f);
    mat4 uiView = lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    
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
    
    std::vector<std::unique_ptr<EquilateralTriangle>> tris;
    
    for(int i = 0; i < 20; i++) {
        tris.push_back(std::make_unique<EquilateralTriangle>(&basicShader, &renderData, &transparentTexture));
        tris[i]->addToTriangleList(&transparentTriangles);
        tris[i]->setTranslation(vec3(float(i / 3.0f) + 2.0f, 0.0f, 0.0f));
        tris[i]->setRotation(vec4(0.0f, 1.0f, 0.0f, HALF_PI));
    }
    
    EquilateralTriangle e(&basicShader, &renderData, &transparentTexture);
    e.setTranslation(vec3(0.0f, 0.0f, -2.0f));
    e.setRotation(vec4(0.0f, 1.0f, 0.0f, HALF_PI));
    e.addToTriangleList(&transparentTriangles);
    
    Cube cube(&basicShader, &renderData, &debug2Texture);
    cube.setTranslation(vec3(3.0f, 4.0f, -1.0f));
    cube.addToTriangleList(&opaqueTriangles);
    
    Cube light(&basicShader, &renderData, &debugTexture);
    light.addToTriangleList(&opaqueTriangles);
    light.setTranslation(vec3(4.0f));
    light.setScale(vec3(0.2f));
    
    Sphere sphere(&basicShader, &renderData, &debugTexture);
    sphere.addToTriangleList(&opaqueTriangles);
    
    ObjModel testModel("resources/model/untitled.obj", &basicShader, &renderData);
    testModel.addToTriangleList(&opaqueTriangles, &transparentTriangles);
    testModel.setTranslation(vec3(-4.0f));
    
    
    UIText text("Hello\nWorld", &uiShader, &uiData);
    text.setScale(vec3(text.getCharDimensions(), 0.0f) * 1.0f);
//    uiTexts.push_back(&text);
    
    UIText fpsText("FPS:   0\nFrametime:   0ms", &uiShader, &uiData);
    fpsText.setScale(vec3(fpsText.getCharDimensions(), 0.0f) * 0.125f);
    uiTexts.push_back(&fpsText);
    
    
    unsigned long triangleAmount = 0;
    
    for(int i = 0; i < opaqueTriangles.size(); i++) {
        triangleAmount += opaqueTriangles[i]->getSize();
    }
    
    printf("%lu of %E possible triangles registerd\n%lu transparent triangles registerd\n%lu opaque triangles registerd\n", transparentTriangles.size() + triangleAmount, double(transparentTriangles.max_size()), transparentTriangles.size(), triangleAmount);
    
    sortThread = std::thread(sortTriangles, &cam, &transparentTriangles);
    
    PointLightSource lightSource(&basicShader);
    lightSource.color = vec3(0.0f, 0.8f, 0.2f);
    lightSource.position = vec3(4.0f);
    
    std::vector<std::unique_ptr<PointLightSource>> lights;
    std::vector<std::unique_ptr<Cube>> lightCubes;
    
    for(int i = 0; i < 10; i++) {
        lights.push_back(std::make_unique<PointLightSource>(&basicShader));
        lights[i]->color = vec3((rand() % 10 / 10.0f), (rand() % 10 / 10.0f), (rand() % 10 / 10.0f));
        lights[i]->position = vec3((rand() % 30) - 15.0f, (rand() % 30) - 15.0f, (rand() % 30) - 15.0f);
        
        lightCubes.push_back(std::make_unique<Cube>(&basicShader, &renderData, &debugTexture));
        lightCubes[i]->addToTriangleList(&opaqueTriangles);
        lightCubes[i]->setTranslation(lights[i]->position);
        lightCubes[i]->setScale(vec3(0.2));
    }
    
    for(int i = 0; i < lights.size(); i++) {
        printf("c\n");
        printVec3(lights[i]->color);
        printf("p\n");
        printVec3(lights[i]->position);
    }

    
    while(running) {
        if(SDL_GetTicks() > nextMeasure) {
            fps = frame;
            frame = 0;
            nextMeasure += 1000;
            
            fpsSum += fps;
            measures++;
            
            
            fpsText.setText("FPS:   " + std::to_string(fps) + "\nFrametime:   " + std::to_string(1000.0f / fps) + "ms");
            
            printf("%d FPS \t\t %f ms average frametime last second\n", fps, 1000.0f / fps);
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
                
                projectionMat = infinitePerspective(radians(cam.getZoom()), float(windowWidth) / float(windowHeight), 0.005f);
                uiProjection = ortho(-0.5f * float(windowWidth), 0.5f * float(windowWidth), -0.5f * float(windowHeight), 0.5f * float(windowHeight), -1000.0f, 1000.0f);
            
                fpsText.setTranslation(glm::vec3(-0.5f * float(windowWidth) + 0.5f * fpsText.getScale().x, 0.5f * float(windowHeight) - 0.5f * fpsText.getScale().y, 0.0f));
                
                glViewport(0, 0, windowWidth, windowHeight);
            }
            
            if(windowEvent.type == SDL_KEYDOWN) {
                if(windowEvent.key.keysym.sym == SDLK_ESCAPE) {
                    render = false;
                    checkMouse = false;
                }
                
                if(windowEvent.key.keysym.sym == SDLK_f)
                    swapBool(&wireframe);
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
            
            sort = true;
            
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            
            if(wireframe == true)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
            
            
            cube.setRotation(vec4(1.0f, 1.0f, 1.0f, tan(totalTime / 5.0f)));
    
            
            for(int i = 0; i < opaqueTriangles.size(); i++) {
                opaqueTriangles[i]->getShaderPointer()->use();
                viewPos.setVar();
                lightSource.activate();
                for(int i = 0; i < lights.size(); i++)
                    lights[i]->activate();
                opaqueTriangles[i]->render();
            }
            
            sphere.setTranslation(vec3(cos(totalTime), sin(totalTime), sin(totalTime)));
            sphere.setRotation(vec4(1.0f, 1.0f, 1.0f, -tan(totalTime / 3.0f)));
            
            
            while(!sortDone)
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            
            for(auto it = transparentTriangles.rbegin(); it != transparentTriangles.rend(); it++) {
                it->second->getShaderPointer()->use();
                viewPos.setVar();
                lightSource.activate();
                for(int i = 0; i < lights.size(); i++)
                    lights[i]->activate();
                it->second->render();
            }
            
            
            
            glClear(GL_DEPTH_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
            for(int i = 0; i < uiTriangles.size(); i++) {
                uiTriangles[i]->getShaderPointer()->use();
                uiTriangles[i]->prepareRender();
                uiTriangles[i]->render();
            }
            
            for(int i = 0; i < uiTexts.size(); i++) {
                uiTexts[i]->render();
            }
            
            
            oldCamFootPos = cam.getFootPosition();
            
            SDL_GL_SwapWindow(window);
            glFlush();
            
            frame++;
            totalFrames++;
            
            totalTime += deltaTime;
        }
        else
            SDL_Delay(33);
    }
    
    sortThread.detach();
    
    printf(PRINTF_RED);
    printf("Average %lu FPS \t\t %f ms average frametime\n", fpsSum / measures, 1000.0f / (fpsSum / measures));
    printf(PRINTF_DEFAULT);
    
    SDL_SetRelativeMouseMode(SDL_FALSE);
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
