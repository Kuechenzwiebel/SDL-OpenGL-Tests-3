//
//  objModel.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 16.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "objModel.hpp"

bool operator==(const std::pair<std::string, std::unique_ptr<Texture>> &l, const std::pair<std::string, std::unique_ptr<Texture>> &r) {
    if(l.first == r.first)
        return true;
    else
        return false;
}

ObjModel::ObjModel(std::string file, Shader *shader, const RenderData *data):
shader(shader), data(data) {
    hg::File inputFile(file);
    
    std::vector<std::string> fileLines = inputFile.readFileLineByLine();
    std::string line;
    std::stringstream lineStream;
    
    std::vector<glm::vec3> readVertices;
    std::vector<glm::vec2> readUVs;
    std::vector<glm::vec3> readNormals;
    
    for(int i = 0; i < fileLines.size(); i++) {
        if(fileLines[i].substr(0, 2) == "o ") {
            objectInfo.push_back(ObjModelComponentInfo{std::make_pair(vertices.size(), 0), hg::substr(file, 0, int(file.find_last_of("/"))) + "/" + hg::substr(fileLines[i], 2, int(fileLines[i].length())) + ".png"});
            if(vertices.size() != 0) {
                objectInfo[objectInfo.size() - 2].objectBounds.second = (unsigned int)vertices.size();
            }
        }
        
        else if(fileLines[i].substr(0, 2) == "v ") {
            float x, y, z;
            lineStream = std::stringstream(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> x >> y >> z;
            readVertices.push_back(glm::vec3(x, y, z));
        }
        
        else if(fileLines[i].substr(0, 2) == "vt") {
            float u, v;
            lineStream = std::stringstream(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> u >> v;
            readUVs.push_back(glm::vec2(u, v));
        }
        
        else if(fileLines[i].substr(0, 2) == "vn") {
            float x, y, z;
            lineStream = std::stringstream(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> x >> y >> z;
            readNormals.push_back(glm::vec3(x, y, z));
        }
        
        else if(fileLines[i].substr(0, 2) == "f ") {
            std::replace(fileLines[i].begin(), fileLines[i].end(), '/', ' ');
            lineStream = std::stringstream(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            
            int v1, v2, v3;
            int t1, t2, t3;
            int n1, n2, n3;
            
            lineStream
            >> v1 >> t1 >> n1
            >> v2 >> t2 >> n2
            >> v3 >> t3 >> n3;
            
            vertices.push_back(readVertices[v1 - 1]);
            vertices.push_back(readVertices[v2 - 1]);
            vertices.push_back(readVertices[v3 - 1]);
            
            uvs.push_back(readUVs[t1 - 1]);
            uvs.push_back(readUVs[t2 - 1]);
            uvs.push_back(readUVs[t3 - 1]);
            
            normals.push_back(readNormals[n1 - 1]);
            normals.push_back(readNormals[n2 - 1]);
            normals.push_back(readNormals[n3 - 1]);
        }
    }
    objectInfo[objectInfo.size() - 1].objectBounds.second = (unsigned int)vertices.size();
    
    int objectIndex = 0;
    for(int i = 0; i < fileLines.size(); i++) {
        if(fileLines[i].substr(0, 2) == "o ") {
            auto texture = std::find(textures.begin(), textures.end(), std::make_pair(objectInfo[objectIndex].textureName, nullptr));
            if(texture == textures.end()) {
                textures.push_back(std::make_pair(objectInfo[objectIndex].textureName, std::make_unique<Texture>(objectInfo[objectIndex].textureName)));
                texture = std::find(textures.begin(), textures.end(), std::make_pair(objectInfo[objectIndex].textureName, nullptr));
            }
            
            if(texture->second->isTransparent()) {
                for(int j = objectInfo[objectIndex].objectBounds.first; j < objectInfo[objectIndex].objectBounds.second; j += 3) {
                    transparentTriangles.push_back(std::make_unique<CoreTriangle>(shader, data, &vertices[j], texture->second.get(), &uvs[j], &normals[j], &modelMat, 32, nullptr, true));
                }
            }
            else {
                opaqueTriangleClusters.push_back(std::make_unique<CoreTriangleCluster>(shader, data, (objectInfo[objectIndex].objectBounds.second - objectInfo[objectIndex].objectBounds.first) / 3, &vertices[objectInfo[objectIndex].objectBounds.first], texture->second.get(), &uvs[objectInfo[objectIndex].objectBounds.first], &normals[objectInfo[objectIndex].objectBounds.first], &modelMat, 32, nullptr, true));
            }
    
            objectIndex++;
        }
    }
}

ObjModel::~ObjModel() {
    
}

void ObjModel::addToTriangleList(std::vector<CoreTriangleCluster*> *oTriangles, std::list<std::pair<float, CoreTriangle*>> *tTriangles) {
    for(int i = 0; i < opaqueTriangleClusters.size(); i++) {
        oTriangles->push_back(opaqueTriangleClusters[i].get());
    }
    
    for(int i = 0; i < transparentTriangles.size(); i++) {
        tTriangles->push_back(std::make_pair(0.0f, transparentTriangles[i].get()));
    }
}
