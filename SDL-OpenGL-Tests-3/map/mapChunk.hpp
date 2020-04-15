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
#include "mapDynamicTriangleCluster.hpp"


#define CHUNK_WIDTH 32
#define TRIANGLE_WIDTH 0.5f
#define CHUNK_TEXTURE_WIDTH 0.25f
#define INVERSE_TRIANGLE_WIDTH 2.0f
#define CHUNK_ARRAY_SIZE (int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)

#define VIEW_RANGE CHUNK_WIDTH * 3

void generateMapData(hg::PerlinNoise *noise, glm::vec3 *mapVertices, glm::vec2 *mapUVs, glm::vec3 *mapNormals, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
void saveMapData(glm::vec3 *mapVertices, glm::vec2 *mapUVs, glm::vec3 *mapNormals);
float mapSurface(glm::vec3 *mapVertices, glm::vec2 position, hg::PerlinNoise *noise);

glm::vec2 chunkGrid(glm::vec2 a);

class MapChunk: public Object {
public:
    MapChunk(Shader *shader, const RenderData *data, Texture *texture, glm::vec3 *mapVertices, glm::vec2 *mapUVs, glm::vec3 *mapNormals);
    ~MapChunk();
    
    void addToTriangleList(std::vector<MapDynamicTriangleCluster*> *triangles);
    
    void setData(glm::vec3 *mapVertices, glm::vec2 *mapUVs, glm::vec3 *mapNormals);
    
    glm::vec2 offset;
    
private:
    MapDynamicTriangleCluster tris;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
    
    std::vector<MapDynamicTriangleCluster*> *trianglePointer;
};

#endif /* mapChunk_hpp */
