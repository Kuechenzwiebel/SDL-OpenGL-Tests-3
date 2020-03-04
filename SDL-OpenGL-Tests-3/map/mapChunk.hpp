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
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>


#include "../coreTriangleCluster.hpp"
#include "../line.hpp"
#include "../object.hpp"


#define CHUNK_WIDTH 32
#define TRIANGLE_WIDTH 0.5f
#define CHUNK_ARRAY_SIZE (int)((CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * (CHUNK_WIDTH * (1.0f / TRIANGLE_WIDTH)) * 6)

#define VIEW_RANGE CHUNK_WIDTH * 4

void generateMapData(hg::PerlinNoise *noise, glm::vec3 *mapVertices, glm::vec2 *mapUVs, glm::vec3 *mapNormals, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

class MapChunk: public Object {
public:
    MapChunk(Shader *shader, const RenderData *data, Texture *texture, glm::vec3 *mapVertices, glm::vec2 *mapUVs, glm::vec3 *mapNormals);
    ~MapChunk();
    
    void addToTriangleList(std::vector<CoreTriangleCluster*> *triangles);
    
    glm::vec2 offset;
    
private:
    std::unique_ptr<CoreTriangleCluster> tris;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
    
    std::vector<CoreTriangleCluster*> *trianglePointer;
};

#endif /* mapChunk_hpp */
