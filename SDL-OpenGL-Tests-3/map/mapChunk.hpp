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
#include <memory>

#include <HG_Noise/HG_Noise.h>

#include "../coreTriangleCluster.hpp"
#include "../object.hpp"

#define CHUNK_WIDTH 32
#define TRIANGLE_WIDTH 1.0f

class MapChunk: public Object {
public:
    MapChunk(Shader *shader, const RenderData *data, Texture *texture, hg::PerlinNoise *noise, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    ~MapChunk();
    
    void addToTriangleList(std::vector<CoreTriangleCluster*> *triangles);
    
private:
    std::unique_ptr<CoreTriangleCluster> tris;
    
    hg::PerlinNoise *noise;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
};

#endif /* mapChunk_hpp */
