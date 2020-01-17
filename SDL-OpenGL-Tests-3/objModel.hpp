//
//  objModel.hpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 16.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#ifndef objModel_hpp
#define objModel_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>

#include <HG_List/HG_List.h>

#include "object.hpp"
#include "coreTriangle.hpp"
#include "coreTriangleCluster.hpp"

class ObjModel: public Object {
public:
    ObjModel(std::string file, Shader *shader, const RenderData *data);
    ~ObjModel();
    
    void addToTriangleList(std::vector<CoreTriangleCluster*> *oTriangles, std::list<std::pair<float, CoreTriangle*>> *tTriangles);
    
private:
    std::vector<std::pair<std::string, std::unique_ptr<Texture>>> textures;
    
    std::unique_ptr<CoreTriangleCluster> opaqueTriangleCluster;
    std::vector<std::unique_ptr<CoreTriangle>> transparentTriangles;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    Shader *shader;
    const RenderData *data;
    Texture *texture;
};

bool operator==(std::pair<std::string, std::unique_ptr<Texture>> &l, std::pair<std::string, std::unique_ptr<Texture>> &r);

#endif /* objModel_hpp */
