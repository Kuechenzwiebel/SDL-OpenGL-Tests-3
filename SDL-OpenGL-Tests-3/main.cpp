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
#include <random>

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
#include "core/shader.hpp"
#include "core/texture.hpp"
#include "core/camera.hpp"
#include "object.hpp"
#include "coreTriangle.hpp"
#include "core/uniformVar.hpp"
#include "equilateralTriangle.hpp"
#include "cube.hpp"
#include "ui/uiRectangle.hpp"
#include "sphere.hpp"
#include "coreTriangleCluster.hpp"
#include "objModel.hpp"
#include "ui/uiText.hpp"
#include "pointLightSource.hpp"
#include "lightSource.hpp"
#include "spotLightSource.hpp"
#include "map/mapChunk.hpp"
#include "line.hpp"

using namespace glm;

int windowWidth = 1080, windowHeight = 760;

std::atomic<bool> running;
bool render = true;
bool wireframe = false;
bool checkMouse = false;


float mouseWheel = 0.0f;


std::thread sortThread;
std::atomic<bool> sortDone;
std::atomic<bool> sort;

glm::vec3 oldCamFootPos;
glm::vec3 oldCamEyePos;

void sortTriangles(Camera *cam, std::list<std::pair<float, CoreTriangle*>> *transparentTriangles) {
    std::cout << "Sort Thread ID: " << std::this_thread::get_id() << std::endl;
    
    while(running) {
        if(sort) {
            sortDone = false;
            
            if(cam->getEyePosition() != oldCamEyePos) {
                for(auto it = transparentTriangles->begin(); it != transparentTriangles->end(); it++) {
                    it->first = glm::distance2(cam->getEyePosition(), it->second->getCenter());
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

std::thread mapUpdateThread;
std::atomic<bool> updateDone;
std::atomic<bool> update;
std::atomic<glm::vec2> offsetReqired;

static std::vector<std::unique_ptr<std::array<glm::vec3, CHUNK_ARRAY_SIZE>>> mapVertices;
static std::vector<std::unique_ptr<std::array<glm::vec2, CHUNK_ARRAY_SIZE>>> mapUVs;
static std::vector<std::unique_ptr<std::array<glm::vec3, CHUNK_ARRAY_SIZE>>> mapNormals;

static std::vector<glm::vec2> requiredRenderChunks;

void mapUpdate(hg::PerlinNoise *noise, Camera *cam) {
    std::cout << "Map Update Thread ID: " << std::this_thread::get_id() << std::endl;
    
    glm::vec2 thisCamPosition = vec2(0.0f), oldCamPosition = vec2(1.0f);
    
    std::vector<glm::vec2> requiredChunks;
    
    glm::vec2 offset;
    requiredChunks.clear();
    requiredRenderChunks.clear();
    
    for(int x = -VIEW_RANGE; x <= VIEW_RANGE; x += CHUNK_WIDTH) {
        for(int y = -VIEW_RANGE; y <= VIEW_RANGE; y += CHUNK_WIDTH) {
            offset = vec2(x, y);
            if(glm::distance(thisCamPosition, thisCamPosition + offset) <= float(VIEW_RANGE)) {
                requiredChunks.emplace_back(thisCamPosition + offset);
                requiredRenderChunks.emplace_back(thisCamPosition + offset);
                
                mapVertices.emplace_back(std::make_unique<std::array<glm::vec3, CHUNK_ARRAY_SIZE>>());
                mapUVs.emplace_back(std::make_unique<std::array<glm::vec2, CHUNK_ARRAY_SIZE>>());
                mapNormals.emplace_back(std::make_unique<std::array<glm::vec3, CHUNK_ARRAY_SIZE>>());
                
                int idx = int(mapVertices.size() - 1);
                generateMapData(noise, mapVertices[idx]->data(), mapUVs[idx]->data(), mapNormals[idx]->data(), requiredChunks[idx]);
            }
        }
    }
    
    mapVertices.shrink_to_fit();
    mapUVs.shrink_to_fit();
    mapNormals.shrink_to_fit();
    
    
    std::cout << "Map Size:" << mapVertices.size() << std::endl;
    
    
    while(running) {
        if(update) {
            updateDone = false;
            
            thisCamPosition = (round(cam->getFootPosition() / vec3(CHUNK_WIDTH)) * vec3(CHUNK_WIDTH)).xz();
            
            glm::vec2 offset;
            requiredChunks.clear();
            requiredRenderChunks.clear();
            
            for(int x = -VIEW_RANGE; x <= VIEW_RANGE; x += CHUNK_WIDTH) {
                for(int y = -VIEW_RANGE; y <= VIEW_RANGE; y += CHUNK_WIDTH) {
                    offset = vec2(x, y);
                    if(glm::distance(thisCamPosition, thisCamPosition + offset) <= float(VIEW_RANGE)) {
                        requiredChunks.emplace_back(thisCamPosition + offset);
                        requiredRenderChunks.emplace_back(thisCamPosition + offset);
                    }
                }
            }
            
            if(thisCamPosition != oldCamPosition) {
                for(int i = 0; i < mapVertices.size(); i++) {
                    auto itr = std::find_if(requiredChunks.begin(), requiredChunks.end(), [i](vec2 &search){return (*mapVertices[i])[0].xz() + vec2(CHUNK_WIDTH) / 2.0f == search;});
                    
                    if(itr == requiredChunks.end()) {
                        mapVertices.erase(mapVertices.begin() + i);
                        i--;
                    }
                    else {
                        requiredChunks.erase(itr);
                    }
                }
                
                for(int i = 0; i < requiredChunks.size(); i++) {
                    mapVertices.emplace_back(std::make_unique<std::array<glm::vec3, CHUNK_ARRAY_SIZE>>());
                    mapUVs.emplace_back(std::make_unique<std::array<glm::vec2, CHUNK_ARRAY_SIZE>>());
                    mapNormals.emplace_back(std::make_unique<std::array<glm::vec3, CHUNK_ARRAY_SIZE>>());
                    
                    int idx = int(mapVertices.size() - 1);
                    generateMapData(noise, mapVertices[idx]->data(), mapUVs[idx]->data(), mapNormals[idx]->data(), requiredChunks[i]);
                }
            }
            
            oldCamPosition = thisCamPosition;
            updateDone = true;
            update = false;
        }
        else {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }
}


int main(int argc, const char * argv[]) {
    std::random_device randomDevice;
    std::mt19937 randomEngine(randomDevice());
    
    std::cout << "Main Thread ID: " << std::this_thread::get_id() << std::endl;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL2");
        return EXIT_FAILURE;
    }
    
    running = true;
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    SDL_Window *window = SDL_CreateWindow("SDL-OpenGL-Tests-3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
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
    std::vector<LightSource*> lightSources;
    
    std::vector<CoreTriangle*> uiTriangles;
    std::vector<UIText*> uiTexts;
    
    hg::File basicShaderVertex("resources/shader/basic.vs"), basicShaderFragment("resources/shader/basic.fs");
    hg::File uiShaderVertex("resources/shader/ui.vs"), uiShaderFragment("resources/shader/ui.fs");
    hg::File colorShaderVertex("resources/shader/colorBuffer.vs"), colorShaderFragment("resources/shader/colorBuffer.fs");
    hg::File diffuseShaderVertex("resources/shader/diffuse.vs"), diffuseShaderFragment("resources/shader/diffuse.fs");
    Shader basicShader(basicShaderVertex, basicShaderFragment);
    Shader uiShader(uiShaderVertex, uiShaderFragment);
    Shader colorBufferShader(colorShaderVertex, colorShaderFragment);
    Shader diffuseShader(diffuseShaderVertex, diffuseShaderFragment);
    
    
    hg::PerlinNoise noise(12345);
    
    noise.octaves = 4;
    noise.frequency = 10.0f;
    noise.multiplier = 2.5f;
    noise.offset = -10.0f;
    
    
    Camera cam(&deltaTime, &windowEvent, &checkMouse, &noise);
    UniformVar<vec3> viewPos(&basicShader, "viewPos", cam.getEyePositionPointer());
    cam.processMouseInput();
    cam.processInput();
    
    mat4 projectionMat = infinitePerspective(radians(cam.zoom), float(windowWidth) / float(windowHeight), 0.005f);
    mat4 uiProjection = ortho(-0.5f * windowWidth, 0.5f * windowWidth, -0.5f * windowHeight, 0.5f * windowHeight, -1.0f, 1.0f);
    mat4 uiView = lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    
    RenderData renderData;
    renderData.projection = &projectionMat;
    renderData.viewMat = &cam.viewMat;
    
    RenderData uiData;
    uiData.projection = &uiProjection;
    uiData.viewMat = &uiView;
    
    
    Texture debugTexture("resources/texture/debug.png");
    Texture debug2Texture("resources/texture/debug2.png");
    Texture stoneTexture("resources/texture/stone.png");
    
    Texture containerTexture("resources/texture/container.png");
    Texture containerReflectionTexture("resources/texture/container_reflection.png", false);
    containerReflectionTexture.textureName = "reflectionMap";
    
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
    blackTexture.textureName = "reflectionMap";
    
    std::vector<std::unique_ptr<EquilateralTriangle>> tris;
    
    for(int i = 0; i < 20; i++) {
        tris.push_back(std::make_unique<EquilateralTriangle>(&basicShader, &renderData, &transparentTexture, 32, nullptr, true));
        tris[i]->addToTriangleList(&transparentTriangles);
        tris[i]->setTranslation(vec3(float(i / 3.0f) + 2.0f, 0.0f, 0.0f));
        tris[i]->setRotation(vec4(0.0f, 1.0f, 0.0f, HALF_PI));
    }
    
    EquilateralTriangle e(&basicShader, &renderData, &transparentTexture, 32, nullptr, true);
    e.setTranslation(vec3(0.0f, 0.0f, -2.0f));
    e.setRotation(vec4(0.0f, 1.0f, 0.0f, HALF_PI));
    e.addToTriangleList(&transparentTriangles);
    
    Cube cube(&basicShader, &renderData, &debug2Texture, 32, nullptr);
    cube.setTranslation(vec3(3.0f, 4.0f, -1.0f));
    cube.addToTriangleList(&opaqueTriangles);
    
    Cube container(&basicShader, &renderData, &containerTexture, 0, &containerReflectionTexture);
    container.addToTriangleList(&opaqueTriangles);
    container.setTranslation(vec3(5.0f, 3.5f, 3.0f));
    
    Cube light(&basicShader, &renderData, &debugTexture, 0, nullptr);
    light.addToTriangleList(&opaqueTriangles);
    light.setTranslation(vec3(4.0f));
    light.setScale(vec3(0.2f));
    
    Sphere sphere(&basicShader, &renderData, &stoneTexture, 32, nullptr);
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
    lightSource.position = vec3(4.0f);
    lightSource.addToLightList(&lightSources);
    
    std::vector<std::unique_ptr<PointLightSource>> lights;
    std::vector<std::unique_ptr<Cube>> lightCubes;
    
    std::uniform_real_distribution<float> lightYPositionDistribution(-8.0f, 5.0f);
    std::uniform_real_distribution<float> lightXZPositionDistribution(-50.0f, 50.0f);
    std::uniform_real_distribution<float> lightColorDistribution(0.2f, 0.7f);
    
    for(int i = 0; i < 20; i++) {
        lights.push_back(std::make_unique<PointLightSource>(&basicShader));
        lights[i]->color = vec3(lightColorDistribution(randomEngine), lightColorDistribution(randomEngine), lightColorDistribution(randomEngine));
        lights[i]->position = vec3(lightXZPositionDistribution(randomEngine), lightYPositionDistribution(randomEngine), lightXZPositionDistribution(randomEngine));
        
        lights[i]->addToLightList(&lightSources);
        
        lightCubes.push_back(std::make_unique<Cube>(&basicShader, &renderData, &debugTexture, 0, nullptr));
        lightCubes[i]->addToTriangleList(&opaqueTriangles);
        lightCubes[i]->setTranslation(lights[i]->position);
        lightCubes[i]->setScale(vec3(0.2));
    }
    
    SpotLightSource flashlight(&basicShader);
    flashlight.addToLightList(&lightSources);
    
    
    int normalView = 0;
    UniformVar<int> normalViewUniform(&basicShader, "normalView", &normalView);
    
    
    
    std::vector<std::unique_ptr<MapChunk>> chunks;
    std::vector<CoreTriangleCluster*> mapTriangles;
    
    mapUpdateThread = std::thread(mapUpdate, &noise, &cam);
    
    update = true;
    
    while(!updateDone)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    while(running) {
        if(SDL_GetTicks() > nextMeasure) {
            fps = frame;
            frame = 0;
            nextMeasure += 1000;
            
            if(render) {
                if(totalFrames > 200) {
                    fpsSum += fps;
                    measures++;
                }
                
                
                fpsText.setText("FPS:   " + std::to_string(fps) + "\nFrametime:   " + std::to_string(1000.0f / fps) + "ms");
                
                printf("%d FPS \t\t %f ms average frametime last second\n", fps, 1000.0f / fps);
            }
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
                
                projectionMat = infinitePerspective(radians(cam.zoom), float(windowWidth) / float(windowHeight), 0.005f);
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
                
                if(windowEvent.key.keysym.sym == SDLK_n)
                    swapBool((bool*)&normalView);
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
            
            sphere.setTranslation(vec3(cos(totalTime), sin(totalTime), sin(totalTime)));
            sphere.setRotation(vec4(1.0f, 1.0f, 1.0f, -tan(totalTime / 3.0f)));
            
            lightSource.color = normalize(vec3(0.0f, 0.8f, 0.2f)) * abs(sin(totalTime / 2.0f));
            
            
            flashlight.position = cam.getEyePosition();
            flashlight.direction = cam.front;
            
            
            for(int i = 0; i < opaqueTriangles.size(); i++) {
                opaqueTriangles[i]->getShaderPointer()->use();
                
                normalViewUniform.setVar();
                viewPos.setVar();
                for(int i = 0; i < lightSources.size(); i++)
                    lightSources[i]->activate();
                
                opaqueTriangles[i]->render();
            }
            
            
            if(updateDone) {
                std::this_thread::sleep_for(std::chrono::microseconds(50));
                
                for(int i = 0; i < chunks.size(); i++) {
                    auto itr = std::find_if(requiredRenderChunks.begin(), requiredRenderChunks.end(), [i, &chunks](vec2 &search){return search == chunks[i]->offset;});
                    
                    if(itr == requiredRenderChunks.end()) {
                        chunks.erase(chunks.begin() + i);
                        i--;
                    }
                    else {
                        requiredRenderChunks.erase(itr);
                    }
                }
                
                
                for(int i = 0; i < requiredRenderChunks.size(); i++) {
                    auto itr = std::find_if(mapVertices.begin(), mapVertices.end(), [i](std::unique_ptr<std::array<glm::vec3, CHUNK_ARRAY_SIZE>> &search){return (*search)[0].xz() + vec2(CHUNK_WIDTH) / 2.0f == requiredRenderChunks[i];});
                    
                    if(itr != mapVertices.end()) {
                        int idx = int(itr - mapVertices.begin());
                        chunks.emplace_back(std::make_unique<MapChunk>(&diffuseShader, &renderData, &stoneTexture, mapVertices[idx]->data(), mapUVs[idx]->data(), mapNormals[idx]->data()));
                        chunks[chunks.size() - 1]->offset = requiredRenderChunks[i];
                        chunks[chunks.size() - 1]->addToTriangleList(&mapTriangles);
                    }
                }
                
                update = true;
            }
            
            
            diffuseShader.use();
            for(int i = 0; i < mapTriangles.size(); i++) {
                normalViewUniform.setVar();
                viewPos.setVar();
                for(int i = 0; i < lightSources.size(); i++)
                    lightSources[i]->activate();
                
                mapTriangles[i]->render();
            }
            
            
            
            while(!sortDone)
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            
            for(auto it = transparentTriangles.rbegin(); it != transparentTriangles.rend(); it++) {
                it->second->getShaderPointer()->use();
                
                normalViewUniform.setVar();
                viewPos.setVar();
                for(int i = 0; i < lightSources.size(); i++)
                    lightSources[i]->activate();
                
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
            oldCamEyePos = cam.getEyePosition();
            
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
    mapUpdateThread.join();
    
    if(fpsSum != 0) {
        printf(PRINTF_RED);
        printf("Average %lu FPS \t\t %f ms average frametime\n", fpsSum / measures, 1000.0f / (fpsSum / measures));
        printf(PRINTF_DEFAULT);
    }
    
    SDL_SetRelativeMouseMode(SDL_FALSE);
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
