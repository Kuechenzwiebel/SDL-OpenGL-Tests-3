//
//  utils.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#define PI 3.1415926535897932384626433f

#define GLM_FORCE_SWIZZLE

#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>

#define PRINTF_RED "\033[0;31m"
#define PRINTF_BLUE "\033[0;34m"
#define PRINTF_GREEN "\033[0;32m"
#define PRINTF_DEFAULT "\033[0m"

void printVec2(glm::vec2 data, bool newLine = true);
void printVec3(glm::vec3 data, bool newLine = true);
void printVec4(glm::vec4 data, bool newLine = true);

template <typename T>
void printArray(T *array, int arraySize);
template <>
void printArray(glm::vec3 *array, int arraySize);

void printMat4x4(glm::mat4 m);

struct RenderData {
    glm::mat4 viewMat;
    glm::mat4 *projection;
    int *windowWidth;
    int *windowHeight;
};

void swapBool(bool *b);

void fadeToZero(float *x, float deltaTime, float timeToZero);

#endif /* utils_hpp */
