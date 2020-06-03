//
//  mapChunk.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 06.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "mapChunk.hpp"


MAP_INDICES_ARRAY_TYPE mapIndices[MAP_INDICES_ARRAY_SIZE] = {0};

static void newlyGenerateMapIndices(std::stringstream &stringstream) {
    stringstream.str("");
    stringstream << CHUNK_SIDE_LENGHT << "\n";
    
    unsigned long arrayIdx = 0;
    unsigned long nextLine = CHUNK_SIDE_LENGHT;
    for(int i = 0; i < CHUNK_ARRAY_SIZE - 1 - CHUNK_SIDE_LENGHT - 1; i++) {
        if(i == nextLine) {
            i++;
            nextLine += CHUNK_SIDE_LENGHT + 1;
            
            stringstream << "\n\n";
        }
        
        mapIndices[arrayIdx + 0] = i;
        mapIndices[arrayIdx + 1] = i + 1;
        mapIndices[arrayIdx + 2] = i + CHUNK_SIDE_LENGHT + 1;
        
        mapIndices[arrayIdx + 5] = mapIndices[arrayIdx + 1];
        mapIndices[arrayIdx + 4] = mapIndices[arrayIdx + 2];
        mapIndices[arrayIdx + 3] = mapIndices[arrayIdx + 2] + 1;
        
        for(int j = 0; j < 6; j++)
            stringstream << mapIndices[arrayIdx + j] << " ";
        
        stringstream << "    ";
        
        arrayIdx += 6;
    }
}


static bool mapIndicesReady = false;

static void generateMapIndices() {
    if(mapIndicesReady == false) {
        mapIndicesReady = true;
        std::string indicesLocation = "/Users/tobiaspfluger/Documents/C/OpenGL/SDL-OpenGL-Tests-3/data/chunkIndices.txt";
        std::stringstream stringstream;
        hg::File file(indicesLocation);
        if(fileExists(indicesLocation)) {
            int loadedSideLenght;
            stringstream << file.readFile();
            stringstream >> loadedSideLenght;
            
            if(loadedSideLenght != CHUNK_SIDE_LENGHT) {
                newlyGenerateMapIndices(stringstream);
                file.writeFile(stringstream.str());
            }
            else
                for(long i = 0; i < MAP_INDICES_ARRAY_SIZE; i++)
                    stringstream >> mapIndices[i];
        }
        else {
            newlyGenerateMapIndices(stringstream);
            file.writeFile(stringstream.str());
        }
    }
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
        
        for(int i = 0;  i < CHUNK_SIDE_LENGHT + 1; i++)
            for(int j = 0; j < CHUNK_SIDE_LENGHT + 1; j++)
                stringstream >> (**mapVertices)[i][j].x >> (**mapVertices)[i][j].y >> (**mapVertices)[i][j].z >> (**mapUVs)[i][j].x >> (**mapUVs)[i][j].y >> (**mapNormals)[i][j].x >> (**mapNormals)[i][j].y >> (**mapNormals)[i][j].z;
    }
    
    else {
        generateMapIndices();
        
        unsigned int arrayX = 0, arrayY = 0;
        for(float x = offset.x - CHUNK_WIDTH / 2.0f; x <= offset.x + CHUNK_WIDTH / 2.0f; x += TRIANGLE_WIDTH) {
            for(float y = offset.y - CHUNK_WIDTH / 2.0f; y <= offset.y + CHUNK_WIDTH / 2.0f; y += TRIANGLE_WIDTH) {
                (**mapVertices)[arrayX][arrayY] = glm::vec3(x, noise->octaveNoise(x, y) * 0.0f, y);
                (**mapUVs)[arrayX][arrayY] = glm::vec2(float(arrayY), float(arrayX)) * INVERSE_CHUNK_TEXTURE_WIDTH * TRIANGLE_WIDTH;
                (**mapNormals)[arrayX][arrayY] = glm::vec3(0.0f, 1.0f, 0.0f);
                
                arrayY++;
            }
            arrayY = 0;
            arrayX ++;
        }
        
        
        glm::vec3 normals[7];
        
        for(int i = 1;  i < CHUNK_SIDE_LENGHT; i++) {
            for(int j = 1; j < CHUNK_SIDE_LENGHT; j++) {
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
        
        
        for(int i = 1; i < CHUNK_SIDE_LENGHT; i++) {
            normals[0] = glm::triangleNormal((**mapVertices)[i][0], (**mapVertices)[i - 1][0], (**mapVertices)[i - 1][0 + 1]);
            normals[1] =-glm::triangleNormal((**mapVertices)[i][0], (**mapVertices)[i][0 + 1], (**mapVertices)[i - 1][0 + 1]);
            normals[2] =-glm::triangleNormal((**mapVertices)[i][0], (**mapVertices)[i + 1][0], (**mapVertices)[i][0 + 1]);
            
            normals[6] = glm::vec3(0.0f);
            
            for(int k = 0; k < 3; k++)
                normals[6] += normals[k];
            
            normals[6] = glm::normalize(normals[6] / 3.0f);
            
            (**mapNormals)[i][0] = normals[6];
        }
        
        for(int i = 1; i < CHUNK_SIDE_LENGHT; i++) {
            normals[0] =-glm::triangleNormal((**mapVertices)[i][CHUNK_SIDE_LENGHT], (**mapVertices)[i - 1][CHUNK_SIDE_LENGHT], (**mapVertices)[i][CHUNK_SIDE_LENGHT - 1]);
            normals[1] = glm::triangleNormal((**mapVertices)[i][CHUNK_SIDE_LENGHT], (**mapVertices)[i + 1][CHUNK_SIDE_LENGHT], (**mapVertices)[i + 1][CHUNK_SIDE_LENGHT - 1]);
            normals[2] =-glm::triangleNormal((**mapVertices)[i][CHUNK_SIDE_LENGHT], (**mapVertices)[i][CHUNK_SIDE_LENGHT - 1], (**mapVertices)[i + 1][CHUNK_SIDE_LENGHT - 1]);
            
            normals[6] = glm::vec3(0.0f);
            
            for(int k = 0; k < 3; k++)
                normals[6] += normals[k];
            
            normals[6] = glm::normalize(normals[6] / 3.0f);
            
            (**mapNormals)[i][CHUNK_SIDE_LENGHT] = normals[6];
        }
        
        
        
        for(int j = 1; j < CHUNK_SIDE_LENGHT; j++) {
            normals[0] =-glm::triangleNormal((**mapVertices)[0][j], (**mapVertices)[0 + 1][j], (**mapVertices)[0][j + 1]);
            normals[1] = glm::triangleNormal((**mapVertices)[0][j], (**mapVertices)[0 + 1][j], (**mapVertices)[0 + 1][j - 1]);
            normals[2] =-glm::triangleNormal((**mapVertices)[0][j], (**mapVertices)[0][j - 1], (**mapVertices)[0 + 1][j - 1]);
            
            normals[6] = glm::vec3(0.0f);
            
            for(int k = 0; k < 3; k++)
                normals[6] += normals[k];
            
            normals[6] = glm::normalize(normals[6] / 3.0f);
            
            (**mapNormals)[0][j] = normals[6];
        }
        
        for(int j = 1; j < CHUNK_SIDE_LENGHT; j++) {
            normals[0] =-glm::triangleNormal((**mapVertices)[CHUNK_SIDE_LENGHT][j], (**mapVertices)[CHUNK_SIDE_LENGHT - 1][j], (**mapVertices)[CHUNK_SIDE_LENGHT][j - 1]);
            normals[1] = glm::triangleNormal((**mapVertices)[CHUNK_SIDE_LENGHT][j], (**mapVertices)[CHUNK_SIDE_LENGHT - 1][j], (**mapVertices)[CHUNK_SIDE_LENGHT - 1][j + 1]);
            normals[2] =-glm::triangleNormal((**mapVertices)[CHUNK_SIDE_LENGHT][j], (**mapVertices)[CHUNK_SIDE_LENGHT][j + 1], (**mapVertices)[CHUNK_SIDE_LENGHT - 1][j + 1]);
            
            normals[6] = glm::vec3(0.0f);
            
            for(int k = 0; k < 3; k++)
                normals[6] += normals[k];
            
            normals[6] = glm::normalize(normals[6] / 3.0f);
            
            (**mapNormals)[CHUNK_SIDE_LENGHT][j] = normals[6];
        }
        
        saveMapData(mapVertices, mapUVs, mapNormals);
    }
}

float mapSurface(MapDataVec3Type *mapVertices, glm::vec2 position, hg::PerlinNoise *noise) {
    return noise->octaveNoise(position.x, position.y);
    
    
    /*
    glm::vec2 mapGridPosition = floor(position / float(TRIANGLE_WIDTH)) * float(TRIANGLE_WIDTH);
    glm::vec2 trianglePositon = position - mapGridPosition;
    
    mapGridPosition += glm::vec2(CHUNK_WIDTH / 2.0f);
    mapGridPosition = glm::mod(mapGridPosition, glm::vec2(CHUNK_WIDTH));
    
    unsigned int arrayX = mapGridPosition.x * INVERSE_TRIANGLE_WIDTH, arrayY = mapGridPosition.y * INVERSE_TRIANGLE_WIDTH;
    
    float z = 0.0f;
    
    printf("%d %d\n", arrayX, arrayY);
    
    
    if(trianglePositon.x + trianglePositon.y > TRIANGLE_WIDTH) {
        z = noise->octaveNoise(position.x, position.y);
        
        if(arrayY - 1 < 0 || arrayX - 1 < 0) {
        z = noise->octaveNoise(position.x, position.y);
            printf("Underflow\n");
        }
        
//        z = -((**mapVertices)[arrayX - 1][arrayY].y - (**mapVertices)[arrayX][arrayY].y) * INVERSE_TRIANGLE_WIDTH * (position.x - (**mapVertices)[arrayX][arrayY].z) +
//            -((**mapVertices)[arrayX][arrayY - 1].y - (**mapVertices)[arrayX][arrayY].y) * INVERSE_TRIANGLE_WIDTH * (position.y - (**mapVertices)[arrayX][arrayY].x) +
//            (**mapVertices)[arrayX][arrayY].y;
    }
    else {
        printf("Calculation\n");
        
        if(arrayY + 1 > CHUNK_SIDE_LENGHT + 1 || arrayX + 1 > CHUNK_SIDE_LENGHT + 1) {
            z = noise->octaveNoise(position.x, position.y);
            printf("Overflow\n");
        }
        else
            
        z = ((**mapVertices)[arrayX + 1][arrayY].y - (**mapVertices)[arrayX][arrayY].y) * INVERSE_TRIANGLE_WIDTH * (position.x - (**mapVertices)[arrayX][arrayY].x) +
            ((**mapVertices)[arrayX][arrayY + 1].y - (**mapVertices)[arrayX][arrayY].y) * INVERSE_TRIANGLE_WIDTH * (position.y - (**mapVertices)[arrayX][arrayY].z) +
            (**mapVertices)[arrayX][arrayY].y;
    }
    
    return z;
     */
}

void saveMapData(MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals) {
    glm::vec2 offset = (**mapVertices)[0][0].xz() + glm::vec2(CHUNK_WIDTH) / 2.0f;
    std::stringstream stringstream, filenamestream;
    filenamestream << "/Users/tobiaspfluger/Documents/C/OpenGL/SDL-OpenGL-Tests-3/data/terrain/chunk   " << offset.x << " " << offset.y << ".txt";
    
    hg::File file(filenamestream.str());
    
    for(int i = 0; i < CHUNK_SIDE_LENGHT + 1; i++)
        for(int j = 0; j < CHUNK_SIDE_LENGHT + 1; j++)
            stringstream << round((**mapVertices)[i][j].x, 3) << " " << round((**mapVertices)[i][j].y, 3) << " " << round((**mapVertices)[i][j].z, 3) << " " << round((**mapUVs)[i][j].x, 3) << " " << round((**mapUVs)[i][j].y, 3) << " " << round((**mapNormals)[i][j].x, 3) << " " << round((**mapNormals)[i][j].y, 3) << " " << round((**mapNormals)[i][j].z, 3) << "\n";
    
    file.writeFile(stringstream.str());
}

glm::vec2 chunkGrid(glm::vec2 a) {
    return glm::round(a / float(CHUNK_WIDTH)) * float(CHUNK_WIDTH);
}

glm::vec2 triangleGrid(glm::vec2 a) {
    return glm::round(a * float(INVERSE_TRIANGLE_WIDTH)) * float(TRIANGLE_WIDTH);
}

float mapMod(float a, float b) {
    float r = fmod(a, b);
    if(r < 0) {
        r = b + r;
    }
    return r;
}



MapChunk::MapChunk(Shader *shader, Texture *texture, MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals):
shader(shader), texture(texture), trianglePointer(nullptr), tris(shader, MAP_INDICES_ARRAY_SIZE, mapIndices, (*mapVertices)->data()->data(), CHUNK_ARRAY_SIZE, texture, (*mapUVs)->data()->data(), (*mapNormals)->data()->data(), &modelMat) {
    
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
