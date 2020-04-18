//
//  mapChunk.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 06.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "mapChunk.hpp"


MAP_INDICES_ARRAY_TYPE mapIndices[MAP_INDICES_ARRAY_SIZE] = {0};

bool mapIndicesReady = false;
void generateMapIndices() {
    if(!mapIndicesReady) {
        std::string indicesLocation = "/Users/tobiaspfluger/Documents/C/OpenGL/SDL-OpenGL-Tests-3/data/chunkIndices.txt";
        std::stringstream stringstream;
        hg::File file(indicesLocation);
        if(fileExists(indicesLocation)) {
            int loadedSideLenght;
            stringstream << file.readFile();
            stringstream >> loadedSideLenght;
            
            if(loadedSideLenght != CHUNK_SIDE_LENGHT)
                goto newlyGenerateMapIndices;
            else
                for(long i = 0; i < MAP_INDICES_ARRAY_SIZE; i++)
                    stringstream >> mapIndices[i];
        }
        else {
        newlyGenerateMapIndices:
            unsigned long arrayIdx = 0;
            for(int i = 0; i < CHUNK_ARRAY_SIZE - 1 - CHUNK_SIDE_LENGHT; i++) {
                if((i % CHUNK_SIDE_LENGHT) == CHUNK_SIDE_LENGHT - 1)
                    i++;
                
                mapIndices[arrayIdx + 0] = i;
                mapIndices[arrayIdx + 1] = i + 1;
                mapIndices[arrayIdx + 2] = i + CHUNK_SIDE_LENGHT;
                
                mapIndices[arrayIdx + 3] = mapIndices[arrayIdx + 1];
                mapIndices[arrayIdx + 4] = mapIndices[arrayIdx + 2];
                mapIndices[arrayIdx + 5] = mapIndices[arrayIdx + 2] + 1;
                
                arrayIdx += 6;
            }
            
            stringstream.str("");
            stringstream << CHUNK_SIDE_LENGHT << "\n";
            
            for(long i = 0; i < MAP_INDICES_ARRAY_SIZE; i++)
                stringstream << mapIndices[i] << " ";
            stringstream << "\n";
            
            file.writeFile(stringstream.str());
        }
    }
    
    mapIndicesReady = true;
}

void generateMapData(hg::PerlinNoise *noise, MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals, glm::vec2 offset) {
    std::stringstream stringstream, filenamestream;
    
    if(offset.x == -0.0f)
        offset.x = 0.0f;
    
    if(offset.y == -0.0f)
        offset.y = 0.0f;
    
    filenamestream << "/Users/tobiaspfluger/Documents/C/OpenGL/SDL-OpenGL-Tests-3/data/terrain/chunk " << offset.x << " " << offset.y << ".txt";
    
    if(fileExists(filenamestream.str())) {
        hg::File file(filenamestream.str());
        stringstream.str(file.readFile());
        
        generateMapIndices();
        
        for(int i = 0;  i < CHUNK_SIDE_LENGHT; i++)
            for(int j = 0; j < CHUNK_SIDE_LENGHT; j++)
                stringstream >> (**mapVertices)[i][j].x >> (**mapVertices)[i][j].y >> (**mapVertices)[i][j].z >> (**mapUVs)[i][j].x >> (**mapUVs)[i][j].y >> (**mapNormals)[i][j].x >> (**mapNormals)[i][j].y >> (**mapNormals)[i][j].z;
    }
    else {
        generateMapIndices();
        
        unsigned int arrayX = 0, arrayY = 0;
        for(float x = offset.x - CHUNK_WIDTH / 2.0f; x < float(CHUNK_WIDTH) + offset.x - CHUNK_WIDTH / 2.0f; x += TRIANGLE_WIDTH) {
            for(float y = offset.y - CHUNK_WIDTH / 2.0f; y < float(CHUNK_WIDTH) + offset.y - CHUNK_WIDTH / 2.0f; y += TRIANGLE_WIDTH) {
                (**mapVertices)[arrayX][arrayY] = glm::vec3(x, noise->octaveNoise(x, y) + 10.0f, y);
                (**mapUVs)[arrayX][arrayY] = glm::vec2(float(arrayY), float(arrayX)) * INVERSE_CHUNK_TEXTURE_WIDTH * TRIANGLE_WIDTH;
                (**mapNormals)[arrayX][arrayY] = glm::vec3(0.0f, 1.0f, 0.0f);
                
                arrayY++;
            }
            arrayY = 0;
            arrayX ++;
        }
        
        glm::vec3 normals[7];
        
        for(int i = 1;  i < CHUNK_SIDE_LENGHT - 1; i++) {
            for(int j = 1; j < CHUNK_SIDE_LENGHT - 1; j++) {
                normals[0] =-glm::triangleNormal((**mapVertices)[i][j], (**mapVertices)[i - 1][j], (**mapVertices)[i][j - 1]);
                normals[1] = glm::triangleNormal((**mapVertices)[i][j], (**mapVertices)[i - 1][j], (**mapVertices)[i - 1][j + 1]);
                normals[2] =-glm::triangleNormal((**mapVertices)[i][j], (**mapVertices)[i][j + 1], (**mapVertices)[i - 1][j + 1]);
                normals[3] =-glm::triangleNormal((**mapVertices)[i][j], (**mapVertices)[i + 1][j], (**mapVertices)[i][j + 1]);
                normals[4] = glm::triangleNormal((**mapVertices)[i][j], (**mapVertices)[i + 1][j], (**mapVertices)[i + 1][j - 1]);
                normals[5] =-glm::triangleNormal((**mapVertices)[i][j], (**mapVertices)[i][j - 1], (**mapVertices)[i + 1][j - 1]);
                
                normals[6] = glm::vec3(0.0f);
                
                for(int k = 0; k < 6; k++)
                    normals[6] += normals[k];
                
                normals[6] = glm::normalize(normals[6] / 6.0f);
                
                (**mapNormals)[i][j] = normals[6];
            }
        }
        
        
        for(int i = 1; i < CHUNK_SIDE_LENGHT - 1; i++) {
            normals[0] = glm::triangleNormal((**mapVertices)[i][0], (**mapVertices)[i - 1][0], (**mapVertices)[i - 1][0 + 1]);
            normals[1] =-glm::triangleNormal((**mapVertices)[i][0], (**mapVertices)[i][0 + 1], (**mapVertices)[i - 1][0 + 1]);
            normals[2] =-glm::triangleNormal((**mapVertices)[i][0], (**mapVertices)[i + 1][0], (**mapVertices)[i][0 + 1]);
            
            normals[6] = glm::vec3(0.0f);
            
            for(int k = 0; k < 3; k++)
                normals[6] += normals[k];
            
            normals[6] = glm::normalize(normals[6] / 3.0f);
            
            (**mapNormals)[i][0] = normals[6];
        }
        
        for(int i = 1; i < CHUNK_SIDE_LENGHT - 1; i++) {
            normals[0] =-glm::triangleNormal((**mapVertices)[i][CHUNK_SIDE_LENGHT - 1], (**mapVertices)[i - 1][CHUNK_SIDE_LENGHT - 1], (**mapVertices)[i][CHUNK_SIDE_LENGHT - 1 - 1]);
            normals[1] = glm::triangleNormal((**mapVertices)[i][CHUNK_SIDE_LENGHT - 1], (**mapVertices)[i + 1][CHUNK_SIDE_LENGHT - 1], (**mapVertices)[i + 1][CHUNK_SIDE_LENGHT - 1 - 1]);
            normals[2] =-glm::triangleNormal((**mapVertices)[i][CHUNK_SIDE_LENGHT - 1], (**mapVertices)[i][CHUNK_SIDE_LENGHT - 1 - 1], (**mapVertices)[i + 1][CHUNK_SIDE_LENGHT - 1 - 1]);
            
            normals[6] = glm::vec3(0.0f);
            
            for(int k = 0; k < 3; k++)
                normals[6] += normals[k];
            
            normals[6] = glm::normalize(normals[6] / 3.0f);
            
            (**mapNormals)[i][CHUNK_SIDE_LENGHT - 1] = normals[6];
        }
        
        
        
        for(int j = 1; j < CHUNK_SIDE_LENGHT - 1; j++) {
            normals[0] =-glm::triangleNormal((**mapVertices)[0][j], (**mapVertices)[0 + 1][j], (**mapVertices)[0][j + 1]);
            normals[1] = glm::triangleNormal((**mapVertices)[0][j], (**mapVertices)[0 + 1][j], (**mapVertices)[0 + 1][j - 1]);
            normals[2] =-glm::triangleNormal((**mapVertices)[0][j], (**mapVertices)[0][j - 1], (**mapVertices)[0 + 1][j - 1]);
            
            normals[6] = glm::vec3(0.0f);
            
            for(int k = 0; k < 3; k++)
                normals[6] += normals[k];
            
            normals[6] = glm::normalize(normals[6] / 3.0f);
            
            (**mapNormals)[0][j] = normals[6];
        }
        
        for(int j = 1; j < CHUNK_SIDE_LENGHT - 1; j++) {
            normals[0] =-glm::triangleNormal((**mapVertices)[CHUNK_SIDE_LENGHT - 1][j], (**mapVertices)[CHUNK_SIDE_LENGHT - 1 - 1][j], (**mapVertices)[CHUNK_SIDE_LENGHT - 1][j - 1]);
            normals[1] = glm::triangleNormal((**mapVertices)[CHUNK_SIDE_LENGHT - 1][j], (**mapVertices)[CHUNK_SIDE_LENGHT - 1 - 1][j], (**mapVertices)[CHUNK_SIDE_LENGHT - 1 - 1][j + 1]);
            normals[2] =-glm::triangleNormal((**mapVertices)[CHUNK_SIDE_LENGHT - 1][j], (**mapVertices)[CHUNK_SIDE_LENGHT - 1][j + 1], (**mapVertices)[CHUNK_SIDE_LENGHT - 1 - 1][j + 1]);
            
            normals[6] = glm::vec3(0.0f);
            
            for(int k = 0; k < 3; k++)
                normals[6] += normals[k];
            
            normals[6] = glm::normalize(normals[6] / 3.0f);
            
            (**mapNormals)[CHUNK_SIDE_LENGHT - 1][j] = normals[6];
        }
       
        saveMapData(mapVertices, mapUVs, mapNormals);
    }
}

float mapSurface(glm::vec3 *mapVertices, glm::vec2 position, hg::PerlinNoise *noise) {
    return 0.0f;
    glm::vec2 mapGridPosition = floor(position / float(TRIANGLE_WIDTH)) * float(TRIANGLE_WIDTH);
    glm::vec2 trianglePositon = position - mapGridPosition;
    
    mapGridPosition += glm::vec2(CHUNK_WIDTH / 2.0f);
    mapGridPosition = glm::mod(mapGridPosition, glm::vec2(CHUNK_WIDTH));
    
    int idx = 6 * (1.0f / TRIANGLE_WIDTH) * (mapGridPosition.y) +
    6 * (1.0f / TRIANGLE_WIDTH) * (mapGridPosition.x) * CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH);
    
    float z = 0.0f;
    
    if(trianglePositon.x + trianglePositon.y > TRIANGLE_WIDTH) {
        if((position.y - mapVertices[idx + 3].z) < -TRIANGLE_WIDTH || (position.y - mapVertices[idx + 3].z) > TRIANGLE_WIDTH)
            z = noise->octaveNoise(position.x, position.y);
        
        else if((position.x - mapVertices[idx + 3].x) < -TRIANGLE_WIDTH || (position.x - mapVertices[idx + 3].x) > TRIANGLE_WIDTH)
            z = noise->octaveNoise(position.x, position.y);
        
        else z = -(mapVertices[idx + 4].y - mapVertices[idx + 3].y) / TRIANGLE_WIDTH * (position.y - mapVertices[idx + 3].z) +
            -(mapVertices[idx + 5].y - mapVertices[idx + 3].y) / TRIANGLE_WIDTH * (position.x - mapVertices[idx + 3].x) +
            mapVertices[idx + 3].y;
    }
    else {
        if((position.y - mapVertices[idx + 0].z) < -TRIANGLE_WIDTH || (position.y - mapVertices[idx + 0].z) > TRIANGLE_WIDTH)
            z = noise->octaveNoise(position.x, position.y);
        
        else if((position.x - mapVertices[idx + 0].x) < -TRIANGLE_WIDTH || (position.x - mapVertices[idx + 0].x) > TRIANGLE_WIDTH)
            z = noise->octaveNoise(position.x, position.y);
        
        else
            z = (mapVertices[idx + 1].y - mapVertices[idx + 0].y) / TRIANGLE_WIDTH * (position.x - mapVertices[idx + 0].x) +
            (mapVertices[idx + 2].y - mapVertices[idx + 0].y) / TRIANGLE_WIDTH * (position.y - mapVertices[idx + 0].z) +
            mapVertices[idx + 0].y;
    }
    
    return z;
}

void saveMapData(MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals) {
     glm::vec2 offset = (**mapVertices)[0][0].xz() + glm::vec2(CHUNK_WIDTH) / 2.0f;
     std::stringstream stringstream, filenamestream;
     filenamestream << "/Users/tobiaspfluger/Documents/C/OpenGL/SDL-OpenGL-Tests-3/data/chunk   " << offset.x << " " << offset.y << ".txt";
     
     hg::File file(filenamestream.str());
     
     for(int i = 0;  i < CHUNK_SIDE_LENGHT; i++)
         for(int j = 0; j < CHUNK_SIDE_LENGHT; j++)
             stringstream << round((**mapVertices)[i][j].x, 3) << " " << round((**mapVertices)[i][j].y, 3) << " " << round((**mapVertices)[i][j].z, 3) << " " << round((**mapUVs)[i][j].x, 3) << " " << round((**mapUVs)[i][j].y, 3) << " " << round((**mapNormals)[i][j].x, 3) << " " << round((**mapNormals)[i][j].y, 3) << " " << round((**mapNormals)[i][j].z, 3) << "\n";
     
     file.writeFile(stringstream.str());
}

glm::vec2 chunkGrid(glm::vec2 a) {
    return glm::round(a / float(CHUNK_WIDTH)) * float(CHUNK_WIDTH);
}

MapChunk::MapChunk(Shader *shader, const RenderData *data, Texture *texture, MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals):
shader(shader), data(data), texture(texture), trianglePointer(nullptr), tris(shader, data, MAP_INDICES_ARRAY_SIZE, mapIndices, (*mapVertices)->data()->data(), CHUNK_ARRAY_SIZE, texture, (*mapUVs)->data()->data(), (*mapNormals)->data()->data(), &modelMat) {
    
}

MapChunk::~MapChunk() {
    if(trianglePointer != nullptr) {
        auto findIter = std::find(trianglePointer->begin(), trianglePointer->end(), &tris);
        if(findIter != trianglePointer->end()) {
            trianglePointer->erase(findIter);
        }
    }
}

void MapChunk::addToTriangleList(std::vector<MapDynamicTriangleElementCluster*> *triangles) {
    triangles->push_back(&tris);
    trianglePointer = triangles;
}

void MapChunk::setData(MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals) {
    tris.setData(mapIndices, (*mapVertices)->data()->data(), (*mapUVs)->data()->data(), (*mapNormals)->data()->data());
}
