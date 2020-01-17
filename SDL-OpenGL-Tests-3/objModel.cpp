//
//  objModel.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 16.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "objModel.hpp"

bool operator==(std::pair<std::string, std::unique_ptr<Texture>> &l, std::pair<std::string, std::unique_ptr<Texture>> &r) {
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
        line = fileLines[i];
        
        if(line.substr(0, 2) == "v ") {
            float x, y, z;
            lineStream.str(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> x >> y >> z;
            readVertices.push_back(glm::vec3(x, y, z));
        }
        
        else if(line.substr(0, 2) == "vt") {
            float u, v;
            lineStream.str(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> u >> v;
            readUVs.push_back(glm::vec2(u, v));
        }
        
        else if(line.substr(0, 2) == "vn") {
            float x, y, z;
            lineStream.str(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            lineStream >> x >> y >> z;
            readNormals.push_back(glm::vec3(x, y, z));
        }
        
        else if(line.substr(0, 2) == "f ") {
            std::replace(fileLines[i].begin(), fileLines[i].end(), '/', ' ');
            lineStream.str(hg::substr(fileLines[i], 2, int(fileLines[i].length())));
            
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
    
    
    for(int i = 0; i < fileLines.size(); i++) {
        line = fileLines[i];
        
        
        if(line.substr(0, 2) == "o ") {
//            if(std::find(textures.begin(), textures.end(), std::make_pair(hg::substr(fileLines[i], 2, int(fileLines[i].length())), nullptr)) == textures.end()) {
//                textures.push_back(std::make_pair(hg::substr(fileLines[i], 2, int(fileLines[i].length())), std::make_unique<Texture>(hg::substr(line, 0, int(line.find_last_of("/"))) + "/" + hg::substr(fileLines[i], 2, int(fileLines[i].length())) + ".png")));
                
//                std::cout << "Opening Texture " << hg::substr(line, 0, int(line.find_last_of("/"))) + "/" + hg::substr(fileLines[i], 2, int(fileLines[i].length())) + ".png" << std::endl;
//            }
            
            
        }
    }
}

ObjModel::~ObjModel() {
    
}

void ObjModel::addToTriangleList(std::vector<CoreTriangleCluster*> *oTriangles, std::list<std::pair<float, CoreTriangle*>> *tTriangles) {
    oTriangles->push_back(opaqueTriangleCluster.get());
    
    for(int i = 0; i < transparentTriangles.size(); i++) {
        tTriangles->push_back(std::make_pair(0.0f, transparentTriangles[i].get()));
    }
}
