//
//  mapChunk.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 06.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "mapChunk.hpp"

static glm::vec3 mapVertices[(int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)] = {glm::vec3(0.0f)};
static glm::vec2 mapUVs[(int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)];
static glm::vec3 mapNormals[(int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)];

MapChunk::MapChunk(Shader *shader, const RenderData *data, Texture *texture, hg::PerlinNoise *noise, glm::vec2 offset):
shader(shader), data(data), texture(texture), trianglePointer(nullptr) {
    int arrayIdx = 0;
    
    for(float x = offset.x; x < float(CHUNK_WIDTH) + offset.x; x += TRIANGLE_WIDTH) {
        for(float y = offset.y; y < float(CHUNK_WIDTH) + offset.y; y += TRIANGLE_WIDTH) {
            float height0 = noise->octaveNoise(x + 0.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH),
                  height1 = noise->octaveNoise(x + 1.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH),
                  height2 = noise->octaveNoise(x + 0.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH),
                  height3 = noise->octaveNoise(x + 1.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
            
            mapVertices[arrayIdx + 0] = glm::vec3(x + 0.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f, height0, y + 0.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f);
            mapVertices[arrayIdx + 1] = glm::vec3(x + 1.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f, height1, y + 0.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f);
            mapVertices[arrayIdx + 2] = glm::vec3(x + 0.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f, height2, y + 1.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f);
            mapVertices[arrayIdx + 3] = glm::vec3(x + 1.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f, height3, y + 1.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f);
            mapVertices[arrayIdx + 4] = glm::vec3(x + 1.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f, height1, y + 0.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f);
            mapVertices[arrayIdx + 5] = glm::vec3(x + 0.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f, height2, y + 1.0f * TRIANGLE_WIDTH - CHUNK_WIDTH / 2.0f);
            
            mapUVs[arrayIdx + 0] = glm::vec2(x + 0.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH);
            mapUVs[arrayIdx + 1] = glm::vec2(x + 1.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH);
            mapUVs[arrayIdx + 2] = glm::vec2(x + 0.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
            mapUVs[arrayIdx + 3] = glm::vec2(x + 1.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
            mapUVs[arrayIdx + 4] = glm::vec2(x + 1.0f * TRIANGLE_WIDTH, y + 0.0f * TRIANGLE_WIDTH);
            mapUVs[arrayIdx + 5] = glm::vec2(x + 0.0f * TRIANGLE_WIDTH, y + 1.0f * TRIANGLE_WIDTH);
            
            mapNormals[arrayIdx + 0] = glm::vec3(0.0f, 1.0f, 0.0f);
            mapNormals[arrayIdx + 1] = glm::vec3(0.0f, 1.0f, 0.0f);
            mapNormals[arrayIdx + 2] = glm::vec3(0.0f, 1.0f, 0.0f);
            mapNormals[arrayIdx + 3] = glm::vec3(0.0f, 1.0f, 0.0f);
            mapNormals[arrayIdx + 4] = glm::vec3(0.0f, 1.0f, 0.0f);
            mapNormals[arrayIdx + 5] = glm::vec3(0.0f, 1.0f, 0.0f);
            
            arrayIdx += 6;
        }
    }
    
    tris = std::make_unique<CoreTriangleCluster>(shader, data, (int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH))) * 2, mapVertices, texture, mapUVs, mapNormals, &modelMat, 32, nullptr, true);
}

void MapChunk::addToTriangleList(std::vector<CoreTriangleCluster*> *triangles) {
    triangles->push_back(tris.get());
}

