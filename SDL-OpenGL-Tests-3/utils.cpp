//
//  utils.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 28.12.19.
//  Copyright © 2019 Tobias Pflüger. All rights reserved.
//

#include "utils.hpp"

void printVec2(glm::vec2 data, bool newLine) {
    printf("X = %f\tY = %f", data.x, data.y);
    if(newLine) {
        printf("\n");
    }
}

void printVec3(glm::vec3 data, bool newLine) {
    printf("X = %f\tY = %f\tZ = %f", data.x, data.y, data.z);
    if(newLine) {
        printf("\n");
    }
}
void printVec4(glm::vec4 data, bool newLine) {
    printf("X = %f\tY = %f\tZ = %f\tW = %f", data.x, data.y, data.z, data.w);
    if(newLine) {
        printf("\n");
    }
}


template <typename T>
void printArray(T *array, int arraySize) {
    for(int i = 0; i < arraySize; i++) {
        std::cout << i << ". = " << array[i] << " ";
    }
    std::cout << std::endl;
}

template <>
void printArray(glm::vec3 *array, int arraySize) {
    for(int i = 0; i < arraySize; i++) {
        std::cout << i << ". = ";
        printVec3(array[i]);
    }
}

void printMat4x4(glm::mat4 m) {
    printf("--------------------------------------------------\n"
           "%f\t%f\t%f\t%f\t\n"
           "%f\t%f\t%f\t%f\t\n"
           "%f\t%f\t%f\t%f\t\n"
           "%f\t%f\t%f\t%f\t\n"
           "--------------------------------------------------\n",
           m[0][0], m[1][0], m[2][0], m[3][0],
           m[0][1], m[1][1], m[2][1], m[3][1],
           m[0][2], m[1][2], m[2][2], m[3][2],
           m[0][3], m[1][3], m[2][3], m[3][3]);
}

void swapBool(bool *b) {
    *b = !(*b);
}

bool fileExists(std::string fileName) {
  struct stat buffer;
  return (stat (fileName.c_str(), &buffer) == 0);
}

int ipow(int x, unsigned int y) {
    unsigned int result = 1;
    for(int i = 0; i < y; i++) {
        result *= x;
    }
    return result;
}

float round(float value, unsigned int digits) {
    int power = ipow(10, digits);
    value *= power;
    value = round(value);
    value /= power;
    return value;
}


glm::mat4 translate(glm::vec3 a) {
    return glm::translate(glm::mat4(1), a);
}

glm::mat4 scale(glm::vec3 a) {
    return glm::scale(glm::mat4(1), a);
}

glm::mat4 rotate(float a, glm::vec3 b) {
    return glm::rotate(glm::mat4(1), a, b);
}


glm::vec3 vectorMatrixMultiplication(glm::vec3 v, glm::mat4 m) {
    return (m * glm::vec4(v, 1.0f)).xyz();
}
