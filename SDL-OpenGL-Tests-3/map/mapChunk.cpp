//
//  mapChunk.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 06.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "mapChunk.hpp"

void generateMapData(hg::PerlinNoise *noise, glm::vec3 *mapVertices, glm::vec2 *mapUVs, glm::vec3 *mapNormals, glm::vec2 offset) {
    std::stringstream stringstream;
    
    if(offset.x == -0.0f)
        offset.x = 0.0f;
    
    if(offset.y == -0.0f)
        offset.y = 0.0f;
    
    if(fileExists("/Users/tobiaspfluger/Desktop/chunk  " + std::to_string(offset.x) + " " + std::to_string(offset.y) + ".txt")) {
        hg::File file("/Users/tobiaspfluger/Desktop/chunk  " + std::to_string(offset.x) + " " + std::to_string(offset.y) + ".txt");
        stringstream.str(file.readFile());
        
        for(int i = 0; i < (int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6); i++)
            stringstream >> mapVertices[i].x >> mapVertices[i].y >> mapVertices[i].z >> mapUVs[i].x >> mapUVs[i].y >> mapNormals[i].x >> mapNormals[i].y >> mapNormals[i].z;
        
        std::cout << "Reading" << std::endl;
    }
    else {
        int arrayIdx = 0;
        
        float height0, height1, height2, height3;
        glm::vec3 normal0, normal1, normal01;
        
        for(float x = offset.x - CHUNK_WIDTH / 2.0f; x < float(CHUNK_WIDTH) + offset.x - CHUNK_WIDTH / 2.0f; x += TRIANGLE_WIDTH) {
            for(float y = offset.y - CHUNK_WIDTH / 2.0f; y < float(CHUNK_WIDTH) + offset.y - CHUNK_WIDTH / 2.0f; y += TRIANGLE_WIDTH) {
                height0 = noise->octaveNoise(x + 0.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH);
                height1 = noise->octaveNoise(x + 1.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH);
                height2 = noise->octaveNoise(x + 0.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
                height3 = noise->octaveNoise(x + 1.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
                
                mapVertices[arrayIdx + 0] = glm::vec3(x + 0.0f * TRIANGLE_WIDTH, height0, y + 0.0f * TRIANGLE_WIDTH);
                mapVertices[arrayIdx + 1] = glm::vec3(x + 1.0f * TRIANGLE_WIDTH, height1, y + 0.0f * TRIANGLE_WIDTH);
                mapVertices[arrayIdx + 2] = glm::vec3(x + 0.0f * TRIANGLE_WIDTH, height2, y + 1.0f * TRIANGLE_WIDTH);
                mapVertices[arrayIdx + 3] = glm::vec3(x + 1.0f * TRIANGLE_WIDTH, height3, y + 1.0f * TRIANGLE_WIDTH);
                mapVertices[arrayIdx + 4] = glm::vec3(x + 1.0f * TRIANGLE_WIDTH, height1, y + 0.0f * TRIANGLE_WIDTH);
                mapVertices[arrayIdx + 5] = glm::vec3(x + 0.0f * TRIANGLE_WIDTH, height2, y + 1.0f * TRIANGLE_WIDTH);
                
                mapUVs[arrayIdx + 0] = glm::vec2(x + 0.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH);
                mapUVs[arrayIdx + 1] = glm::vec2(x + 1.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH);
                mapUVs[arrayIdx + 2] = glm::vec2(x + 0.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
                mapUVs[arrayIdx + 3] = glm::vec2(x + 1.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
                mapUVs[arrayIdx + 4] = glm::vec2(x + 1.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH);
                mapUVs[arrayIdx + 5] = glm::vec2(x + 0.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
                
                
                
                normal0 = -1.0f * glm::triangleNormal(mapVertices[arrayIdx + 0], mapVertices[arrayIdx + 1], mapVertices[arrayIdx + 2]);
                normal1 =  1.0f * glm::triangleNormal(mapVertices[arrayIdx + 3], mapVertices[arrayIdx + 4], mapVertices[arrayIdx + 5]);
                normal01 = glm::normalize((normal0 + normal1) / 2.0f);
                
                mapNormals[arrayIdx + 0] = normal0;
                mapNormals[arrayIdx + 1] = normal01;
                mapNormals[arrayIdx + 2] = normal01;
                mapNormals[arrayIdx + 3] = normal1;
                mapNormals[arrayIdx + 4] = normal01;
                mapNormals[arrayIdx + 5] = normal01;
                
                arrayIdx += 6;
            }
        }
        
        arrayIdx = 0;
        
        glm::vec3 averageNormals;
        
        for(float x = offset.x - CHUNK_WIDTH / 2.0f; x < float(CHUNK_WIDTH) + offset.x - CHUNK_WIDTH / 2.0f; x += TRIANGLE_WIDTH) {
            for(float y = offset.y - CHUNK_WIDTH / 2.0f; y < float(CHUNK_WIDTH) + offset.y - CHUNK_WIDTH / 2.0f; y += TRIANGLE_WIDTH) {
                if(x == float(CHUNK_WIDTH) + offset.x - CHUNK_WIDTH / 2.0f - TRIANGLE_WIDTH && y == float(CHUNK_WIDTH) + offset.y - CHUNK_WIDTH / 2.0f - TRIANGLE_WIDTH) {
                    
                }
                else if(x == float(CHUNK_WIDTH) + offset.x - CHUNK_WIDTH / 2.0f - TRIANGLE_WIDTH) {
                    averageNormals = glm::normalize((mapNormals[arrayIdx + 3 + 0] +
                                                     mapNormals[arrayIdx + 1 + 6] +
                                                     mapNormals[arrayIdx + 4 + 6])
                                                    / 3.0f);
                    
                    mapNormals[arrayIdx + 3 + 0] = averageNormals;
                    mapNormals[arrayIdx + 1 + 6] = averageNormals;
                    mapNormals[arrayIdx + 4 + 6] = averageNormals;
                }
                else if(y == float(CHUNK_WIDTH) + offset.y - CHUNK_WIDTH / 2.0f - TRIANGLE_WIDTH) {
                    averageNormals = glm::normalize((mapNormals[arrayIdx + 3 + 0] +
                                                     mapNormals[arrayIdx + 2 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)] +
                                                     mapNormals[arrayIdx + 5 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)])
                                                    / 3.0f);
                    
                    mapNormals[arrayIdx + 3 + 0] = averageNormals;
                    mapNormals[arrayIdx + 2 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)] = averageNormals;
                    mapNormals[arrayIdx + 5 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)] = averageNormals;
                }
                else {
                    averageNormals = glm::normalize((mapNormals[arrayIdx + 3 + 0] +
                                                     mapNormals[arrayIdx + 1 + 6] +
                                                     mapNormals[arrayIdx + 4 + 6] +
                                                     mapNormals[arrayIdx + 2 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)] +
                                                     mapNormals[arrayIdx + 5 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)] +
                                                     mapNormals[arrayIdx + 0 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6 + 6)])
                                                    / 6.0f);
                    
                    mapNormals[arrayIdx + 3 + 0] = averageNormals;
                    mapNormals[arrayIdx + 1 + 6] = averageNormals;
                    mapNormals[arrayIdx + 4 + 6] = averageNormals;
                    mapNormals[arrayIdx + 2 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)] = averageNormals;
                    mapNormals[arrayIdx + 5 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)] = averageNormals;
                    mapNormals[arrayIdx + 0 + int(round(CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6 + 6)] = averageNormals;
                }
                
                arrayIdx += 6;
            }
        }
        
        std::cout << "Generating" << std::endl;
        
        hg::File file("/Users/tobiaspfluger/Desktop/chunk  " + std::to_string(offset.x) + " " + std::to_string(offset.y) + ".txt");
        
        for(int i = 0; i < (int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6); i++)
            stringstream << mapVertices[i].x << " " << mapVertices[i].y << " " << mapVertices[i].z << " " << mapUVs[i].x << " " << mapUVs[i].y << " " << mapNormals[i].x << " " << mapNormals[i].y << " " << mapNormals[i].z << "\n";
        
        file.writeFile(stringstream.str());
    }
}

MapChunk::MapChunk(Shader *shader, const RenderData *data, Texture *texture, glm::vec3 *mapVertices, glm::vec2 *mapUVs, glm::vec3 *mapNormals):
shader(shader), data(data), texture(texture), trianglePointer(nullptr) {
    tris = std::make_unique<CoreTriangleCluster>(shader, data, (int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH))) * 2, mapVertices, texture, mapUVs, mapNormals, &modelMat, 4, nullptr, true);
}

MapChunk::~MapChunk() {
    if(trianglePointer != nullptr) {
        auto findIter = std::find(trianglePointer->begin(), trianglePointer->end(), tris.get());
        if(findIter != trianglePointer->end()) {
            trianglePointer->erase(findIter);
        }
    }
}

void MapChunk::addToTriangleList(std::vector<CoreTriangleCluster*> *triangles) {
    triangles->push_back(tris.get());
    trianglePointer = triangles;
}

