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
#include <glm/gtc/integer.hpp>

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
#include "ray.hpp"
#include "core/buffers/uniformBuffer.hpp"

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

std::vector<MapDataVec3Type> mapVertices;
std::vector<MapDataVec2Type> mapUVs;
std::vector<MapDataVec3Type> mapNormals;

std::thread mapUpdateThread;
std::mutex mapUpdateMutex;

std::atomic<bool> mapUpdateDone, doMapUpdate;
std::vector<glm::vec2> chunksToErase;
std::vector<uint8_t> chunksToCreate;
std::vector<glm::vec2> loadedRenderMapChunks;


void updateMap(Camera *cam, hg::PerlinNoise *noise) {
    std::cout << "Map Update Thread ID: " << std::this_thread::get_id() << std::endl;
    
    std::vector<glm::vec2> requiredChunks;
    std::vector<uint8_t> freeChunkArrayIndices;
    
    while(running) {
        if(doMapUpdate) {
            mapUpdateDone = false;
            
            requiredChunks.clear();
            
            vec2 thisCamPosition = chunkGrid(cam->getFootPosition().xz());
            
            for(int x = -VIEW_RANGE; x <= VIEW_RANGE; x += CHUNK_WIDTH) {
                for(int y = -VIEW_RANGE; y <= VIEW_RANGE; y += CHUNK_WIDTH) {
                    vec2 offset(x, y);
                    if(glm::length(offset) <= float(VIEW_RANGE)) {
                        requiredChunks.emplace_back(thisCamPosition + offset);
                    }
                }
            }
            
            mapUpdateMutex.lock();
            
            
            for(int i = 0; i < mapVertices.size(); i++) {
                if((*mapVertices[i])[0][0] != vec3(-1.0f)) {
                    vec2 chunkMiddle = (*mapVertices[i])[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f);
                    auto searchItr = std::find(requiredChunks.begin(), requiredChunks.end(), chunkMiddle);
                    
                    if(searchItr == requiredChunks.end()) {
                        (*mapVertices[i])[0][0] = vec3(-1.0f);
                        
                        auto renderSearchItr = std::find(loadedRenderMapChunks.begin(), loadedRenderMapChunks.end(), chunkMiddle);
                        
                        if(renderSearchItr != loadedRenderMapChunks.end())
                            chunksToErase.emplace_back(chunkMiddle);
                        
                        freeChunkArrayIndices.emplace_back(i);
                    }
                    else {
                        requiredChunks.erase(searchItr);
                    }
                }
            }
            
            
            for(int i = 0; i < requiredChunks.size(); i++) {
                if(freeChunkArrayIndices.size() > 0) {
                    int freeIdx = freeChunkArrayIndices[0];
                    generateMapData(noise, &mapVertices[freeIdx], &mapUVs[freeIdx], &mapNormals[freeIdx], requiredChunks[i]);
                    
                    freeChunkArrayIndices.erase(freeChunkArrayIndices.begin());
                    
                    chunksToCreate.emplace_back(freeIdx);
                }
                else {
                    mapVertices.emplace_back(std::make_unique<MapDataRawVec3Type>());
                    mapUVs.emplace_back(std::make_unique<MapDataRawVec2Type>());
                    mapNormals.emplace_back(std::make_unique<MapDataRawVec3Type>());
                    
                    int newIdx = (int)mapVertices.size() - 1;
                    
                    generateMapData(noise, &mapVertices[newIdx], &mapUVs[newIdx], &mapNormals[newIdx], requiredChunks[i]);
                    
                    chunksToCreate.emplace_back(newIdx);
                }
            }
            
            mapUpdateMutex.unlock();
            
            mapUpdateDone = true;
            doMapUpdate = false;
        }
        else {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }
}


int main(int argc, const char * argv[]) {
    std::random_device randomDevice;
    std::mt19937 randomEngine(3928);
    
    bool capFps = true;
    int fpsCap = 60;
    double frameTimeCap = double(1e6) / double(fpsCap);
    std::chrono::steady_clock::time_point start, end;
    
    
    std::cout << "Main Thread ID: " << std::this_thread::get_id() << std::endl;
    
    
    /*
     OpenGL and SDL setup
     */
    
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
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
    
    SDL_GL_SetSwapInterval(0);
    
    if(window == NULL) {
        printf(PRINTF_RED);
        printf("Failed to open Window!\n");
        printf(PRINTF_DEFAULT);
        return EXIT_FAILURE;
    }
    
    glewExperimental = GL_TRUE;
    
    if(glewInit() != GLEW_OK) {
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
    
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    
    
    /*
     Performance measure setup
     */
    
    
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
    
    
    /*
     Vector and list setup
     */
    
    std::list<std::pair<float, CoreTriangle*>> transparentTriangles;
    std::vector<CoreTriangleCluster*> opaqueTriangles;
    std::vector<LightSource*> lightSources;
    std::vector<Line*> lines;
    
    std::vector<CoreTriangle*> uiTriangles;
    std::vector<UIText*> uiTexts;
    
    
    
    /*
     Shader setup
     */
    
    hg::File basicShaderVertex("resources/shader/basic.vs"), basicShaderFragment("resources/shader/basic.fs");
    hg::File uiShaderVertex("resources/shader/ui.vs"), uiShaderFragment("resources/shader/ui.fs");
    hg::File colorShaderVertex("resources/shader/colorBuffer.vs"), colorShaderFragment("resources/shader/colorBuffer.fs");
    hg::File diffuseShaderVertex("resources/shader/diffuse.vs"), diffuseShaderFragment("resources/shader/diffuse.fs");
    Shader basicShader(basicShaderVertex, basicShaderFragment);
    Shader uiShader(uiShaderVertex, uiShaderFragment);
    Shader colorBufferShader(colorShaderVertex, colorShaderFragment);
    Shader diffuseShader(diffuseShaderVertex, diffuseShaderFragment);
    
    
    
    /*
     Perlin noise setup
     */
    
    hg::MultiPerlinNoise noise;
    
    noise.perlinNoises.push_back(std::make_unique<hg::PerlinNoise>((12345)));
    noise.perlinNoises.push_back(std::make_unique<hg::PerlinNoise>((123456)));
    
    noise.perlinNoises[0]->octaves = 4;
    noise.perlinNoises[0]->frequency = 10.0f;
    noise.perlinNoises[0]->multiplier = 2.5f;
    noise.perlinNoises[0]->offset = -10.0f;
    
    noise.perlinNoises[1]->octaves = 2;
    noise.perlinNoises[1]->frequency = 100.0f;
    noise.perlinNoises[1]->multiplier = 25.0f;
    noise.perlinNoises[1]->offset = -10.0f;
    
    
    /*
     Camera setup
     */
    
    vec2 chunkGridCameraPosition(0.0f), mapGridCameraPosition(0.0f);
    
    Camera cam(&deltaTime, &windowEvent, &checkMouse, &noise);
    cam.processMouseInput();
    
    
    
    /*
     Uniform variable setup
     */
    
    UniformBuffer projViewBuffer(2 * sizeof(mat4), 0);
    UniformBuffer uiProjViewBuffer(2 * sizeof(mat4), 1);
    UniformBuffer baseInformationUniform(4 * sizeof(float), 2);
    UniformBuffer shakeUniform(sizeof(float), 3);
    
    projViewBuffer.addToShader(&basicShader, "projView", 0);
    projViewBuffer.addToShader(&colorBufferShader, "projView", 0);
    projViewBuffer.addToShader(&diffuseShader, "projView", 0);
    
    uiProjViewBuffer.addToShader(&uiShader, "uiProjView", 1);
    
    baseInformationUniform.addToShader(&basicShader, "baseInformation", 2);
    baseInformationUniform.addToShader(&diffuseShader, "baseInformation", 2);
    
    shakeUniform.addToShader(&basicShader, "shakeInfo", 3);
    shakeUniform.addToShader(&diffuseShader, "shakeInfo", 3);
    
    
    /*
     Matrices and RenderData setup
     */
    
    mat4 projectionMat = infinitePerspective(radians(cam.zoom), float(windowWidth) / float(windowHeight), 0.005f);
    mat4 uiProjection = ortho(-0.5f * windowWidth, 0.5f * windowWidth, -0.5f * windowHeight, 0.5f * windowHeight, -1.0f, 1.0f);
    mat4 uiView = lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    
    projViewBuffer.modifyData(sizeof(mat4), 0, glm::value_ptr(projectionMat));
    uiProjViewBuffer.modifyData(sizeof(mat4), 0, glm::value_ptr(uiProjection));
    uiProjViewBuffer.modifyData(sizeof(mat4), sizeof(mat4), glm::value_ptr(uiView));
    
    
    /*
     Texture loading
     */
    
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
    
    
    /*
     Basic object generation
     */
    
    std::vector<std::unique_ptr<EquilateralTriangle>> tris;
    
    for(int i = 0; i < 20; i++) {
        tris.push_back(std::make_unique<EquilateralTriangle>(&basicShader, &transparentTexture, 32, nullptr, true));
        tris[i]->addToTriangleList(&transparentTriangles);
        tris[i]->setTranslation(vec3(float(i / 3.0f) + 2.0f, 0.0f, 0.0f));
        tris[i]->setRotation(vec4(0.0f, 1.0f, 0.0f, HALF_PI));
    }
    
    EquilateralTriangle e(&basicShader, &transparentTexture, 32, nullptr, true);
    e.setTranslation(vec3(0.0f, 0.0f, -2.0f));
    e.setRotation(vec4(0.0f, 1.0f, 0.0f, HALF_PI));
    e.addToTriangleList(&transparentTriangles);
    
    Cube cube(&basicShader, &debug2Texture, 32, nullptr);
    cube.setTranslation(vec3(3.0f, 4.0f, -1.0f));
    cube.addToTriangleList(&opaqueTriangles);
    
    Cube container(&basicShader, &containerTexture, 0, &containerReflectionTexture);
    container.addToTriangleList(&opaqueTriangles);
    container.setTranslation(vec3(5.0f, 3.5f, 3.0f));
    
    Cube light(&basicShader, &debugTexture, 0, nullptr);
    light.addToTriangleList(&opaqueTriangles);
    light.setTranslation(vec3(4.0f));
    light.setScale(vec3(0.2f));
    
    Sphere sphere(&basicShader, &stoneTexture, 32, nullptr);
//    sphere.addToTriangleList(&opaqueTriangles);
    
    
    /*
     UI elements
     */
    
    UIText fpsText("FPS:   0\nFrametime:   0ms", &uiShader);
    fpsText.setScale(vec3(fpsText.getCharDimensions(), 0.0f) * 0.125f);
    uiTexts.push_back(&fpsText);
    
    UIText positionText("Position: X = 0.0  Y = 0.0  Z = 0.0", &uiShader);
    positionText.setScale(vec3(positionText.getCharDimensions(), 0.0f) * 0.125f);
    uiTexts.push_back(&positionText);
    
    UIText vehicleSpeedText("Velocity: 0.0 m/s", &uiShader);
    vehicleSpeedText.setScale(vec3(positionText.getCharDimensions(), 0.0f) * 0.125f);
    
    
    float shakeStrenght = 0.0f, explosionShakeStrenght = 0.0f;
    
    
    /*
     Transparent object sorting thread
     */
    
    sortThread = std::thread(sortTriangles, &cam, &transparentTriangles);
    
    
    /*
     Light source generation
     */
    
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
        
        lightCubes.push_back(std::make_unique<Cube>(&basicShader, &debugTexture, 0, nullptr));
        lightCubes[i]->addToTriangleList(&opaqueTriangles);
        lightCubes[i]->setTranslation(lights[i]->position);
        lightCubes[i]->setScale(vec3(0.2));
    }
    
    SpotLightSource flashlight(&basicShader);
    flashlight.addToLightList(&lightSources);
    
    
    int viewMode = 0;
    UniformVar<int> viewModeUniform(&basicShader, "viewMode", &viewMode);
    
    
    
    /*
     Mapupdate variables
     */
    
    
    std::vector<std::unique_ptr<MapChunk>> mapChunks;
    std::vector<MapDynamicTriangleElementCluster*> mapTriangleClusters;
    
    std::thread mapUpdateThread(updateMap, &cam, &noise);
    
    
    doMapUpdate = true;
    while(!mapUpdateDone)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    

    auto middleItr = std::find_if(mapVertices.begin(), mapVertices.end(), [chunkGridCameraPosition](MapDataVec3Type &search){ return chunkGridCameraPosition == (*search)[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f); });
    int middleIdx = int(middleItr - mapVertices.begin());
    
    
    
    /*
     Map editing variables
     */
    
    int mouseRayChunkIndex = 0, mouseRayDataIndex = 0;
    int mouseRayArrayIndexX = 0, mouseRayArrayIndexY = 0;
    
    unsigned int materialCount = 0;
    
    float mapHeight = 0.0f;
    
    bool mapChange = false;
    
    vec2 mouseRayMapPosition, mouseRayChunkPosition, mouseRayRelativePosition;
    bool mapChangeDown = true;
    
    Ray mouseRay;
    bool mouseRayMapCollision = false;
    
    float mapEditDelta = 1.0f;
    
    
    float explosionOffset = 0.0f;
    
    
    
    /*
     Vehicle variables
     */
    
    ObjModel vehicleBase("resources/model/vehicle/vehicle.obj", &basicShader);
    vehicleBase.addToTriangleList(&opaqueTriangles, &transparentTriangles);
    
    
    ObjModel axisRear("resources/model/vehicle/axisRear.obj", &basicShader);
    axisRear.addToTriangleList(&opaqueTriangles, &transparentTriangles);
    
    
    ObjModel axisFront("resources/model/vehicle/axisFront.obj", &basicShader);
    axisFront.addToTriangleList(&opaqueTriangles, &transparentTriangles);
    
    ObjModel wheel_L("resources/model/vehicle/wheel_L.obj", &basicShader);
    wheel_L.addToTriangleList(&opaqueTriangles, &transparentTriangles);
    
    ObjModel wheel_R("resources/model/vehicle/wheel_R.obj", &basicShader);
    wheel_R.addToTriangleList(&opaqueTriangles, &transparentTriangles);
    
    
    int vehicleChunkIdx = 0;
    unsigned long vehicleLastOnFloor = SDL_GetTicks();
    bool camInVehicle = false;
    bool vehicleOnFloor = false;
    
    float frontWheelAngle = 0.0f, lastFrontWheelAngle = 0.0f;
    float vehicleYAngle = 0.0f, vehicleXAngle = 0.0f, vehicleZAngle = 0.0f;
    float vehicleVelocity = 0.0f;
    
    float frontAxisAngle = 0.0f, rearAxisAngle = 0.0f;
    vec3 wheelFL, wheelFR, wheelRL, wheelRR;
    vec3 axisF, axisR;
    
    
    vec3 vehicleBasePosition(0.0f);
    vec2 vehicleBaseChunkGrid;
    
    std::stringstream vehicleSpeedStringStream;
    
    
    while(running) {
        start = std::chrono::steady_clock::now();
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
            if (windowEvent.type == SDL_QUIT)
                running = false;
            
            if(windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                if(checkMouse) {
                    mouseRayMapCollision = false;
                    vec2 lastMouseRayPosition;
                    
                    for(int i = 0; i < 250; i++) {
                        mouseRay.move(0.1f);
                        
                        mouseRayChunkPosition = chunkGrid(mouseRay.position.xz());
                        
                        if(mouseRayChunkPosition != lastMouseRayPosition)
                            mouseRayChunkIndex = int(std::find_if(mapChunks.begin(), mapChunks.end(), [mouseRayChunkPosition](std::unique_ptr<MapChunk> &search){return search->offset  == mouseRayChunkPosition;}) - mapChunks.begin());
                        
                        mouseRayDataIndex = int(std::find_if(mapVertices.begin(), mapVertices.end(), [mouseRayChunkPosition](MapDataVec3Type &search){return (*search)[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f)  == mouseRayChunkPosition;}) - mapVertices.begin());
                        
                        lastMouseRayPosition = mouseRayChunkPosition;
                        
                        
                        if(mouseRay.position.y <= mapSurface(nullptr, mouseRay.position.xz(), &noise)) {
                            if(mouseRayChunkIndex < mapChunks.size() && mouseRayDataIndex < mapVertices.size())
                                mouseRayMapCollision = true;
                            
                            
                            mouseRayMapPosition = triangleGrid(mouseRay.position.xz());
                            
                            mouseRayArrayIndexX = mapMod(mouseRayMapPosition.x + CHUNK_WIDTH * 0.5f, CHUNK_WIDTH) * INVERSE_TRIANGLE_WIDTH;
                            mouseRayArrayIndexY = mapMod(mouseRayMapPosition.y + CHUNK_WIDTH * 0.5f, CHUNK_WIDTH) * INVERSE_TRIANGLE_WIDTH;
                            
                            mouseRayRelativePosition = mouseRayMapPosition - mouseRayChunkPosition;
                            
                            break;
                        }
                    }
                }
                
                if(windowEvent.button.button == SDL_BUTTON_LEFT) {
                    if(mouseRayMapCollision && checkMouse) {
                        if(mouseRayArrayIndexX != 0) {
                            mapHeight = (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][mouseRayArrayIndexY].y;
                            mapHeight -= mapEditDelta;
                        }
                        materialCount++;
                        mapChange = true;
                        mapChangeDown = true;
                    }
                    
                    checkMouse = true;
                    render = true;
                }
                
                if(windowEvent.button.button == SDL_BUTTON_RIGHT) {
                    if(mouseRayMapCollision && checkMouse && materialCount > 0) {
                        if(mouseRayArrayIndexX != 0) {
                            mapHeight = (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][mouseRayArrayIndexY].y;
                            mapHeight += mapEditDelta;
                        }
                        materialCount--;
                        mapChange = true;
                        mapChangeDown = false;
                    }
                }
                
                if(mapChange) {
                    if(mouseRayArrayIndexX == 0) {
                        if(mouseRayRelativePosition.x < 0.0f) {
                            if(mapChangeDown) {
                                mapHeight = (*mapVertices[mouseRayDataIndex])[0][mouseRayArrayIndexY].y;
                                mapHeight -= mapEditDelta;
                                (*mapVertices[mouseRayDataIndex])[0][mouseRayArrayIndexY].y = mapHeight;
                            }
                            else {
                                mapHeight = (*mapVertices[mouseRayDataIndex])[0][mouseRayArrayIndexY].y;
                                mapHeight += mapEditDelta;
                                (*mapVertices[mouseRayDataIndex])[0][mouseRayArrayIndexY].y = mapHeight;
                            }
                            
                            glm::vec2 neighborChunkPosition = mouseRayChunkPosition - vec2(CHUNK_WIDTH, 0.0f);
                            
                            int neighborChunkIndex = int(std::find_if(mapChunks.begin(), mapChunks.end(), [neighborChunkPosition](std::unique_ptr<MapChunk> &search){return search->offset  == neighborChunkPosition;}) - mapChunks.begin());
                            
                            int neighborDataIndex = int(std::find_if(mapVertices.begin(), mapVertices.end(), [neighborChunkPosition](MapDataVec3Type &search){return (*search)[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f)  == neighborChunkPosition;}) - mapVertices.begin());
                            
                            (*mapVertices[neighborDataIndex])[CHUNK_SIDE_LENGHT][mouseRayArrayIndexY].y = mapHeight;
                            mapChunks[neighborChunkIndex]->setData(&mapVertices[neighborDataIndex], &mapUVs[neighborDataIndex], &mapNormals[neighborDataIndex]);
                        }
                        else {
                            if(mapChangeDown) {
                                mapHeight = (*mapVertices[mouseRayDataIndex])[CHUNK_SIDE_LENGHT][mouseRayArrayIndexY].y;
                                mapHeight -= mapEditDelta;
                                (*mapVertices[mouseRayDataIndex])[CHUNK_SIDE_LENGHT][mouseRayArrayIndexY].y = mapHeight;
                            }
                            else {
                                mapHeight = (*mapVertices[mouseRayDataIndex])[CHUNK_SIDE_LENGHT][mouseRayArrayIndexY].y;
                                mapHeight += mapEditDelta;
                                (*mapVertices[mouseRayDataIndex])[CHUNK_SIDE_LENGHT][mouseRayArrayIndexY].y = mapHeight;
                            }
                            
                            glm::vec2 neighborChunkPosition = mouseRayChunkPosition + vec2(CHUNK_WIDTH, 0.0f);
                            
                            int neighborChunkIndex = int(std::find_if(mapChunks.begin(), mapChunks.end(), [neighborChunkPosition](std::unique_ptr<MapChunk> &search){return search->offset  == neighborChunkPosition;}) - mapChunks.begin());
                            
                            int neighborDataIndex = int(std::find_if(mapVertices.begin(), mapVertices.end(), [neighborChunkPosition](MapDataVec3Type &search){return (*search)[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f)  == neighborChunkPosition;}) - mapVertices.begin());
                            
                            (*mapVertices[neighborDataIndex])[0][mouseRayArrayIndexY].y = mapHeight;
                            mapChunks[neighborChunkIndex]->setData(&mapVertices[neighborDataIndex], &mapUVs[neighborDataIndex], &mapNormals[neighborDataIndex]);
                        }
                    }
                    else if(mouseRayArrayIndexY == 0) {
                        if(mouseRayRelativePosition.y < 0.0f) {
                            if(mapChangeDown) {
                                mapHeight = (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][0].y;
                                mapHeight -= mapEditDelta;
                                (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][0].y = mapHeight;
                            }
                            else {
                                mapHeight = (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][0].y;
                                mapHeight += mapEditDelta;
                                (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][0].y = mapHeight;
                            }
                            
                            glm::vec2 neighborChunkPosition = mouseRayChunkPosition - vec2(0.0f, CHUNK_WIDTH);
                            
                            int neighborChunkIndex = int(std::find_if(mapChunks.begin(), mapChunks.end(), [neighborChunkPosition](std::unique_ptr<MapChunk> &search){return search->offset  == neighborChunkPosition;}) - mapChunks.begin());
                            
                            int neighborDataIndex = int(std::find_if(mapVertices.begin(), mapVertices.end(), [neighborChunkPosition](MapDataVec3Type &search){return (*search)[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f)  == neighborChunkPosition;}) - mapVertices.begin());
                            
                            (*mapVertices[neighborDataIndex])[mouseRayArrayIndexX][CHUNK_SIDE_LENGHT].y = mapHeight;
                            mapChunks[neighborChunkIndex]->setData(&mapVertices[neighborDataIndex], &mapUVs[neighborDataIndex], &mapNormals[neighborDataIndex]);
                        }
                        else {
                            if(mapChangeDown) {
                                mapHeight = (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][CHUNK_SIDE_LENGHT].y;
                                mapHeight -= mapEditDelta;
                                (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][CHUNK_SIDE_LENGHT].y = mapHeight;
                            }
                            else {
                                mapHeight = (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][CHUNK_SIDE_LENGHT].y;
                                mapHeight += mapEditDelta;
                                (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][CHUNK_SIDE_LENGHT].y = mapHeight;
                            }
                            
                            glm::vec2 neighborChunkPosition = mouseRayChunkPosition + vec2(0.0f, CHUNK_WIDTH);
                            
                            int neighborChunkIndex = int(std::find_if(mapChunks.begin(), mapChunks.end(), [neighborChunkPosition](std::unique_ptr<MapChunk> &search){return search->offset  == neighborChunkPosition;}) - mapChunks.begin());
                            
                            int neighborDataIndex = int(std::find_if(mapVertices.begin(), mapVertices.end(), [neighborChunkPosition](MapDataVec3Type &search){return (*search)[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f)  == neighborChunkPosition;}) - mapVertices.begin());
                            
                            (*mapVertices[neighborDataIndex])[mouseRayArrayIndexX][0].y = mapHeight;
                            mapChunks[neighborChunkIndex]->setData(&mapVertices[neighborDataIndex], &mapUVs[neighborDataIndex], &mapNormals[neighborDataIndex]);
                        }
                    }
                    else {
                        (*mapVertices[mouseRayDataIndex])[mouseRayArrayIndexX][mouseRayArrayIndexY].y = mapHeight;
                    }
                    
                    mapChunks[mouseRayChunkIndex]->setData(&mapVertices[mouseRayDataIndex], &mapUVs[mouseRayDataIndex], &mapNormals[mouseRayDataIndex]);
                    
                    
                    mapChange = false;
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
                projViewBuffer.modifyData(sizeof(mat4), 0, glm::value_ptr(projectionMat));
                uiProjViewBuffer.modifyData(sizeof(mat4), 0, glm::value_ptr(uiProjection));
                
                fpsText.setTranslation(glm::vec3(-(0.5f * float(windowWidth) - 0.5f * fpsText.getScale().x), (0.5f * float(windowHeight) - 0.5f * fpsText.getScale().y), 0.0f));
                positionText.setTranslation(glm::vec3(-(0.5f * float(windowWidth) - 0.5f * positionText.getScale().x), -(0.5f * float(windowHeight) - 0.5f * positionText.getScale().y), 0.0f));
                
                glViewport(0, 0, windowWidth, windowHeight);
            }
            
            if(windowEvent.type == SDL_KEYDOWN) {
                if(windowEvent.key.keysym.sym == SDLK_ESCAPE) {
                    render = false;
                    checkMouse = false;
                }
                
                if(windowEvent.key.keysym.sym == SDLK_f)
                    swapBool(&wireframe);
                
                if(windowEvent.key.keysym.sym == SDLK_n) {
                    viewMode++;
                    viewMode = viewMode % 3;
                }
                
                if(windowEvent.key.keysym.sym == SDLK_v)
                    swapBool(&camInVehicle);
                
                if(windowEvent.key.keysym.sym == SDLK_r) {
                    vehicleBasePosition.y = 0.0f;
                    vehicleLastOnFloor = SDL_GetTicks();
                }
                
                if(windowEvent.key.keysym.sym == SDLK_e) {
                    shakeStrenght = 1.2f;
                    explosionOffset = totalTime;
                }
            }
            
            if(windowEvent.type == SDL_MOUSEWHEEL) {
                mouseWheel += float(windowEvent.wheel.y) * 0.15f;
                
                vehicleZAngle += float(windowEvent.wheel.y) * 0.15f;
                frontAxisAngle += float(windowEvent.wheel.x) * 0.15f;
            }
        }
        
        if(checkMouse)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
        
        if(render) {
            /*
             Vehicle handling
             */
            
            
            if(camInVehicle) {
                const Uint8 *keystates = SDL_GetKeyboardState(NULL);
                
                if(keystates[SDL_SCANCODE_W])
                    vehicleVelocity += 6.125f * deltaTime;
                
                if(keystates[SDL_SCANCODE_A])
                    frontWheelAngle += 10.0f * deltaTime;
                
                if(keystates[SDL_SCANCODE_S])
                    vehicleVelocity -= 6.125f * deltaTime;
                
                if(keystates[SDL_SCANCODE_D])
                    frontWheelAngle -= 10.0f * deltaTime;
                
                if(keystates[SDL_SCANCODE_SPACE]) {
                    if(vehicleVelocity < -1.0f)
                        vehicleVelocity += 20.0f * deltaTime;
                    else if(vehicleVelocity > 1.0f)
                        vehicleVelocity -= 20.0f * deltaTime;
                    else
                        vehicleVelocity = 0.0f;
                }
            }
            
            
            if(vehicleOnFloor)
                vehicleYAngle += frontWheelAngle * vehicleVelocity * deltaTime;
            else
                vehicleYAngle += lastFrontWheelAngle * vehicleVelocity * deltaTime;
            
            vehicleBasePosition.x += cos(radians(vehicleYAngle)) * vehicleVelocity * deltaTime;
            vehicleBasePosition.z -= sin(radians(vehicleYAngle)) * vehicleVelocity * deltaTime;
            
            if(frontWheelAngle < -30.0f)
                frontWheelAngle = -30.0f;
            
            if(frontWheelAngle > 30.0f)
                frontWheelAngle = 30.0f;
            
            
            if(vehicleOnFloor) {
                if(frontWheelAngle < -0.1f)
                    frontWheelAngle += 0.5f * deltaTime * vehicleVelocity;
                else if(frontWheelAngle > 0.1f)
                    frontWheelAngle -= 0.5f * deltaTime * vehicleVelocity;
                else
                    frontWheelAngle = 0.0f;
            }
            
            
            
            vehicleBaseChunkGrid = round(vehicleBasePosition.xz() / float(CHUNK_WIDTH)) * float(CHUNK_WIDTH);
            vehicleChunkIdx = 0;/*int(std::find_if(chunks.begin(), chunks.end(), [&vehicleBaseChunkGrid](std::unique_ptr<MapChunk> &search){return search->offset == vehicleBaseChunkGrid;}) - chunks.begin());*/
    
            
            float vehicleBaseMapHeight = 0.0f /*mapSurface(mapVertices[vehicleChunkIdx]->data(), vehicleBasePosition.xz(), &noise)*/;
            if(vehicleBasePosition.y - 0.76f > vehicleBaseMapHeight) {
                vehicleBasePosition.y -= 0.5f * gravitationalAcceleration * pow((SDL_GetTicks() - vehicleLastOnFloor) / 1000.0f, 2.0f) * deltaTime;
                vehicleOnFloor = false;
            }
            else {
                vehicleLastOnFloor = SDL_GetTicks();
                vehicleBasePosition.y = vehicleBaseMapHeight + 0.76f;
                vehicleOnFloor = true;
            }
            
            wheelFL = vehicleBasePosition + vectorMatrixMultiplication(vec3(1.5215f, 0.0f, 0.7161f)  , rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)));
            wheelFR = vehicleBasePosition + vectorMatrixMultiplication(vec3(1.5215f, 0.0f, -0.7161f) , rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)));
            wheelRL = vehicleBasePosition + vectorMatrixMultiplication(vec3(-1.5215f, 0.0f, 0.7161f) , rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)));
            wheelRR = vehicleBasePosition + vectorMatrixMultiplication(vec3(-1.5215f, 0.0f, -0.7161f), rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)));
            
            axisF = vehicleBasePosition +   vectorMatrixMultiplication(vec3(1.5215f, 0.0f, 0.0f)     , rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)));
            axisR = vehicleBasePosition +   vectorMatrixMultiplication(vec3(-1.5215f, 0.0f, 0.0f)    , rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)));
            
            /*
            if(chunkGrid(wheelFL) == vehicleBaseChunkGrid)
                wheelFL.y = mapSurface(mapVertices[vehicleChunkIdx]->data(), wheelFL.xz(), &noise);
            else {
                int idx = int(std::find_if(chunks.begin(), chunks.end(), [&wheelFL](std::unique_ptr<MapChunk> &search){return search->offset == chunkGrid(wheelFL);}) - chunks.begin());
                wheelFL.y = mapSurface(mapVertices[idx]->data(), wheelFL.xz(), &noise);
            }
            
            if(chunkGrid(wheelFR) == vehicleBaseChunkGrid)
                wheelFR.y = mapSurface(mapVertices[vehicleChunkIdx]->data(), wheelFR.xz(), &noise);
            else {
                int idx = int(std::find_if(chunks.begin(), chunks.end(), [&wheelFR](std::unique_ptr<MapChunk> &search){return search->offset == chunkGrid(wheelFR);}) - chunks.begin());
                wheelFR.y = mapSurface(mapVertices[idx]->data(), wheelFR.xz(), &noise);
            }
            
            if(chunkGrid(wheelRL) == vehicleBaseChunkGrid)
                wheelRL.y = mapSurface(mapVertices[vehicleChunkIdx]->data(), wheelRL.xz(), &noise);
            else {
                int idx = int(std::find_if(chunks.begin(), chunks.end(), [&wheelRL](std::unique_ptr<MapChunk> &search){return search->offset == chunkGrid(wheelRL);}) - chunks.begin());
                wheelRL.y = mapSurface(mapVertices[idx]->data(), wheelRL.xz(), &noise);
            }
            
            if(chunkGrid(wheelRR) == vehicleBaseChunkGrid)
                wheelRR.y = mapSurface(mapVertices[vehicleChunkIdx]->data(), wheelRR.xz(), &noise);
            else {
                int idx = int(std::find_if(chunks.begin(), chunks.end(), [&wheelRR](std::unique_ptr<MapChunk> &search){return search->offset == chunkGrid(wheelRR);}) - chunks.begin());
                wheelRR.y = mapSurface(mapVertices[idx]->data(), wheelRR.xz(), &noise);
            }
            
            
            if(chunkGrid(axisF) == vehicleBaseChunkGrid)
                axisF.y = mapSurface(mapVertices[vehicleChunkIdx]->data(), axisF.xz(), &noise);
            else {
                int idx = int(std::find_if(chunks.begin(), chunks.end(), [&axisF](std::unique_ptr<MapChunk> &search){return search->offset == chunkGrid(axisF);}) - chunks.begin());
                axisF.y = mapSurface(mapVertices[idx]->data(), axisF.xz(), &noise);
            }
            
            if(chunkGrid(axisR) == vehicleBaseChunkGrid)
                axisR.y = mapSurface(mapVertices[vehicleChunkIdx]->data(), axisR.xz(), &noise);
            else {
                int idx = int(std::find_if(chunks.begin(), chunks.end(), [&axisR](std::unique_ptr<MapChunk> &search){return search->offset == chunkGrid(axisR);}) - chunks.begin());
                axisR.y = mapSurface(mapVertices[idx]->data(), axisR.xz(), &noise);
            }
            */
            
            
            if(vehicleOnFloor) {
                frontAxisAngle =(-atan((wheelFL.y - axisF.y) / 0.7161f) + atan((wheelFR.y - axisF.y) / 0.7161f)) * 0.5f;
                rearAxisAngle = (-atan((wheelRL.y - axisR.y) / 0.7161f) + atan((wheelRR.y - axisR.y) / 0.7161f)) * 0.5f;
                
                vehicleXAngle = (rearAxisAngle + frontAxisAngle) * 0.5f;
                
                frontAxisAngle = clamp(frontAxisAngle, vehicleXAngle - radians(9.9f), vehicleXAngle + radians(9.9f));
                rearAxisAngle = clamp(rearAxisAngle, vehicleXAngle - radians(9.9f), vehicleXAngle + radians(9.9f));
            }
            
            
            
            vehicleBasePosition.y = (axisF.y + axisR.y) * 0.5f;
            
            vehicleZAngle = atan((axisF.y - vehicleBasePosition.y) / 1.5215f);
            
            vehicleBasePosition.y += 0.76f * 0.5f;
            
            
            vehicleBase.setModelMat(translate(vehicleBasePosition) * rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(vehicleZAngle, vec3(0.0f, 0.0f, 1.0f)) * rotate(vehicleXAngle, vec3(1.0f, 0.0f, 0.0f)));
            
            axisRear.setModelMat (translate(vehicleBasePosition) * rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(vehicleZAngle, vec3(0.0f, 0.0f, 1.0f)) * rotate(rearAxisAngle,  vec3(1.0f, 0.0f, 0.0f)) * translate(vec3(-1.5215f, 0.0f, 0.0f)));
            
            axisFront.setModelMat(translate(vehicleBasePosition) * rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(vehicleZAngle, vec3(0.0f, 0.0f, 1.0f)) * rotate(frontAxisAngle, vec3(1.0f, 0.0f, 0.0f)) * translate(vec3(1.5215f, 0.0f, 0.0f)));
            wheel_R.setModelMat  (translate(vehicleBasePosition) * rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(vehicleZAngle, vec3(0.0f, 0.0f, 1.0f)) * rotate(frontAxisAngle, vec3(1.0f, 0.0f, 0.0f)) * translate(vec3(1.5215f, 0.0f, -0.7161f)) * rotate(radians(frontWheelAngle), vec3(0.0f, 1.0f, 0.0f)));
            wheel_L.setModelMat  (translate(vehicleBasePosition) * rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(vehicleZAngle, vec3(0.0f, 0.0f, 1.0f)) * rotate(frontAxisAngle, vec3(1.0f, 0.0f, 0.0f)) * translate(vec3(1.5215f, 0.0f, 0.7161f))  * rotate(radians(frontWheelAngle), vec3(0.0f, 1.0f, 0.0f)));
            
            
            
            cam.keyBoardControl = !camInVehicle;
            
            if(vehicleOnFloor)
                lastFrontWheelAngle = frontWheelAngle;
            
            if(camInVehicle)
                cam.setEyePosition(vectorMatrixMultiplication(vec3(0.35f, 0.45f + 0.97f, -0.45f), translate(mat4(1), vehicleBasePosition) *
                                                              rotate(radians(vehicleYAngle), vec3(0.0f, 1.0f, 0.0f)) *
                                                              rotate(vehicleZAngle, vec3(0.0f, 0.0f, 1.0f)) *
                                                              rotate(vehicleXAngle, vec3(1.0f, 0.0f, 0.0f))));
            
            
            
            /*
             Camera - Chunk handling
             */
            
            cam.preProcessInput();

            chunkGridCameraPosition = chunkGrid(cam.getFootPosition().xz());
            mapGridCameraPosition = triangleGrid(cam.getFootPosition().xz());
            
            mapGridCameraPosition += glm::vec2(CHUNK_WIDTH / 2.0f);
            mapGridCameraPosition = glm::mod(mapGridCameraPosition, glm::vec2(CHUNK_WIDTH));
            
            mouseRay.position = cam.getEyePosition();
            mouseRay.direction = cam.front;
            
            cam.processInput(nullptr);
            
            
            
            /*
             Start transparent triangle sorting
             */
            
            sort = true;
            
            
            
            /*
             Sending global uniforms
             */
            
            if(explosionShakeStrenght > 0.0f)
                explosionShakeStrenght = -0.5f * pow((totalTime - explosionOffset), 3) + 1.5f;
            else
                explosionShakeStrenght = 0.0f;
            
            shakeStrenght = explosionShakeStrenght + cam.velocity * 0.003f;
            
            shakeUniform.modifyData(sizeof(float), 0, &shakeStrenght);
            
            baseInformationUniform.modifyData(sizeof(float), 0, &totalTime);
            baseInformationUniform.modifyData(sizeof(float), sizeof(float) * 1, &(cam.getEyePositionPointer()->x));
            baseInformationUniform.modifyData(sizeof(float), sizeof(float) * 2, &(cam.getEyePositionPointer()->y));
            baseInformationUniform.modifyData(sizeof(float), sizeof(float) * 3, &(cam.getEyePositionPointer()->z));
            
            projViewBuffer.modifyData(sizeof(mat4), sizeof(mat4), glm::value_ptr(cam.viewMat));
            
            
            
            /*
             Positions updating
             */
            
            if(oldCamFootPos != cam.getFootPosition()) {
                std::stringstream positionStream;
                positionStream << std::fixed << std::setprecision(2) << "Position: X = " << cam.getFootPosition().x << "  Y = " <<   cam.getFootPosition().y << "  Z = " << cam.getFootPosition().z;
                positionText.setText(positionStream.str());
            }
            
            /*
             OpenGL settings
             */
            
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            
            if(wireframe == true)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
            
            
            /*
             Individual object settings
             */
            
            cube.setRotation(vec4(1.0f, 1.0f, 1.0f, tan(totalTime / 5.0f)));
            
            sphere.setTranslation(vec3(cos(totalTime), sin(totalTime), sin(totalTime)));
            sphere.setRotation(vec4(1.0f, 1.0f, 1.0f, -tan(totalTime / 3.0f)));
            
            lightSource.color = normalize(vec3(0.0f, 0.8f, 0.2f)) * abs(sin(totalTime / 2.0f));
            
            
            flashlight.position = cam.getEyePosition();
            flashlight.direction = cam.front;
            
            
            /*
             Rendering of opaque triangles
             */
            
            for(int i = 0; i < opaqueTriangles.size(); i++) {
                opaqueTriangles[i]->getShaderPointer()->use();
                
                viewModeUniform.setVar();
                for(int j = 0; j < lightSources.size(); j++)
                    lightSources[j]->activate();
                
                opaqueTriangles[i]->render();
            }
            
            
            /*
             Rendering colorbuffer objects
             */
            
            colorBufferShader.use();
            for(int i = 0; i < lines.size(); i++) {
                lines[i]->render();
            }
            
            
            
            /*
             Map and Chunk handling
             */
            
            middleItr = std::find_if(mapVertices.begin(), mapVertices.end(), [chunkGridCameraPosition](MapDataVec3Type &search){ return chunkGridCameraPosition == (*search)[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f); });
            middleIdx = int(middleItr - mapVertices.begin());
            
            if(middleItr == mapVertices.end()) {
                printf("Waiting for Map Update\n");
                
                while(!mapUpdateDone) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                
                printf("Map Update ready\n");
            }
            
            
            if(mapUpdateDone) {
                for(int i = 0; i < chunksToErase.size(); i++) {
                    vec2 searchedChunk = chunksToErase[i];
                    auto searchItr = std::find_if(mapChunks.begin(), mapChunks.end(), [searchedChunk](std::unique_ptr<MapChunk> &search){ return search->offset == searchedChunk; });
                    
                    if(searchItr != mapChunks.end())
                        mapChunks.erase(searchItr);
                }
                chunksToErase.clear();
                
                
                for(int i = 0; i < chunksToCreate.size(); i++) {
                    uint8_t arrayIdx = chunksToCreate[i];
                    mapChunks.emplace_back(std::make_unique<MapChunk>(&diffuseShader, &stoneTexture, &mapVertices[arrayIdx], &mapUVs[arrayIdx], &mapNormals[arrayIdx]));
                    mapChunks[mapChunks.size() - 1]->offset = (*mapVertices[arrayIdx])[0][0].xz() + vec2(CHUNK_WIDTH / 2.0f);
                    mapChunks[mapChunks.size() - 1]->addToTriangleList(&mapTriangleClusters);
                }
                chunksToCreate.clear();
                
                loadedRenderMapChunks.clear();
                loadedRenderMapChunks.resize(mapChunks.size());
                
                for(int i = 0; i < mapChunks.size(); i++) {
                    loadedRenderMapChunks[i] = mapChunks[i]->offset;
                }
                
                doMapUpdate = true;
            }
            
            
            /*
             Map rendering
             */
            
            //glEnable(GL_CULL_FACE);
            
            diffuseShader.use();
            for(int i = 0; i < mapTriangleClusters.size(); i++) {
                viewModeUniform.setVar();
                for(int j = 0; j < lightSources.size(); j++)
                    lightSources[j]->activate();
                
                mapTriangleClusters[i]->render();
            }
            
            
            glDisable(GL_CULL_FACE);
            
            
            
            /*
             Transparent object rendering
             */
            
            while(!sortDone)
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            
            
            for(auto it = transparentTriangles.rbegin(); it != transparentTriangles.rend(); it++) {
                it->second->getShaderPointer()->use();
                
                viewModeUniform.setVar();
                for(int j = 0; j < lightSources.size(); j++)
                    lightSources[j]->activate();
                
                it->second->render();
            }
            
            
            /*
             UI drawing
             */
            
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
            
            
            /*
             Verhicle UI handling
             */
            
            if(camInVehicle) {
                vehicleSpeedStringStream.str("");
                vehicleSpeedStringStream << "Velocity: " << std::fixed << std::setprecision(2) << vehicleVelocity << "m/s";
                
                vehicleSpeedText.setTranslation(glm::vec3((0.5f * float(windowWidth) - 0.5f * vehicleSpeedText.getScale().x) - (int(vehicleSpeedStringStream.tellp()) - 1) * vehicleSpeedText.getScale().x, (0.5f * float(windowHeight) - 0.5f * vehicleSpeedText.getScale().y), 0.0f));
                
                vehicleSpeedStringStream << "\n       ";
                
                if(vehicleVelocity * 3.6f <= -10.0f)
                    vehicleSpeedStringStream << " ";
                if(vehicleVelocity * 3.6f < 0.0f && vehicleVelocity * 3.6f > -10.0f)
                    vehicleSpeedStringStream << "  ";
                else if(vehicleVelocity * 3.6f < 10.0f && vehicleVelocity * 3.6f >= 0.0f)
                    vehicleSpeedStringStream << "  ";
                else if(vehicleVelocity * 3.6f >= 10.0f)
                    vehicleSpeedStringStream << " ";
                vehicleSpeedStringStream << vehicleVelocity * 3.6f << "km/h";
                
                
                vehicleSpeedText.setText(vehicleSpeedStringStream.str());
                
                vehicleSpeedText.render();
            }
            
            
                
            /*
             Performance recording
             */
            
            oldCamFootPos = cam.getFootPosition();
            oldCamEyePos = cam.getEyePosition();
            
            SDL_GL_SwapWindow(window);
            glFlush();
            
            frame++;
            totalFrames++;
            
            totalTime += deltaTime;
            
            end = std::chrono::steady_clock::now();
            
            if(capFps) {
                int delayTime = round(frameTimeCap - std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) - 300;
                if(delayTime > 10)
                    std::this_thread::sleep_for(std::chrono::microseconds(delayTime));
            }
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
