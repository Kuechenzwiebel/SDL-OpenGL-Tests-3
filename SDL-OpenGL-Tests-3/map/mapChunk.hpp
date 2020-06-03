//
//  mapChunk.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 06.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef mapChunk_hpp
#define mapChunk_hpp

#include <stdio.h>
#include <vector>
#include <array>
#include <memory>

#include <HG_Noise/HG_Noise.h>
#include <HG_List/HG_List.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>


#include "../coreTriangleCluster.hpp"
#include "../line.hpp"
#include "../object.hpp"
#include "mapDynamicTriangleElementCluster.hpp"


#include "mapDefines.hpp"

typedef std::array<std::array<glm::vec3, CHUNK_SIDE_LENGHT + 1>, CHUNK_SIDE_LENGHT + 1> MapDataRawVec3Type;
typedef std::array<std::array<glm::vec2, CHUNK_SIDE_LENGHT + 1>, CHUNK_SIDE_LENGHT + 1> MapDataRawVec2Type;

typedef std::unique_ptr<std::array<std::array<glm::vec3, CHUNK_SIDE_LENGHT + 1>, CHUNK_SIDE_LENGHT + 1>> MapDataVec3Type;
typedef std::unique_ptr<std::array<std::array<glm::vec2, CHUNK_SIDE_LENGHT + 1>, CHUNK_SIDE_LENGHT + 1>> MapDataVec2Type;

void generateMapData(hg::PerlinNoise *noise, MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
void saveMapData(MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals);
float mapSurface(MapDataVec3Type *mapVertices, glm::vec2 position, hg::PerlinNoise *noise);

glm::vec2 chunkGrid(glm::vec2 a);
glm::vec2 triangleGrid(glm::vec2 a);
float mapMod(float a, float b);

class MapChunk: public Object {
public:
    MapChunk(Shader *shader, Texture *texture, MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals);
    ~MapChunk();
    
    void addToTriangleList(std::vector<MapDynamicTriangleElementCluster*> *triangles);
    
    void setData(MapDataVec3Type *mapVertices, MapDataVec2Type *mapUVs, MapDataVec3Type *mapNormals);
    
    glm::vec2 offset;
    
private:
    MapDynamicTriangleElementCluster tris;
    
    Shader *shader;
    Texture *texture;
    
    std::vector<MapDynamicTriangleElementCluster*> *trianglePointer;
};

#endif /* mapChunk_hpp */
