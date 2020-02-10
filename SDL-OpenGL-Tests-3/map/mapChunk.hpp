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

#include "../coreTriangleCluster.hpp"
#include "../object.hpp"

#define CHUNK_WIDTH 32
#define TRIANGLE_WIDTH 2.0f


class MapChunk: public Object {
public:
    MapChunk(Shader *shader, const RenderData *data, Texture *texture);
    
    void addToTriangleList(std::vector<CoreTriangleCluster*> *triangles);
    
private:
    std::unique_ptr<CoreTriangleCluster> tris;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
};

#endif /* mapChunk_hpp */
