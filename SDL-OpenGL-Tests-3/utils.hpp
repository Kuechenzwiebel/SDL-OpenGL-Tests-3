//
//  utils.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#define DOUBLE_PI 6.283185307179586f
#define PI 3.1415926535897932384626433f
#define HALF_PI 1.570796326794897f


#define GLM_FORCE_SWIZZLE

#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sys/stat.h>

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

void swapBool(bool *b);

bool fileExists(std::string fileName);

int ipow(int x, unsigned int y);
float round(float value, unsigned int digits);


static float gravitationalAcceleration = 9.81f;
static float cameraMovementSpeed = 5.0f;


glm::mat4 translate(glm::vec3 a);
glm::mat4 scale(glm::vec3 a);
glm::mat4 rotate(float a, glm::vec3 b);

glm::vec3 vectorMatrixMultiplication(glm::vec3 v, glm::mat4 m);

#endif /* utils_hpp */
