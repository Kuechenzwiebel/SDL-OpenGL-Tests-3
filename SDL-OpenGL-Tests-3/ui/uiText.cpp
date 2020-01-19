//
//  uiText.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 18.01.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "uiText.hpp"

static std::vector<hg::HGList> asciiBandInfo = hg::getHGListsFromFileName("resources/font/asciiBandInfo.hg");

static glm::vec2 asciiBandCharUVs[2][3] = {
    {   glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)},
    
    {   glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)}
};

static void prepareAsciiBandUVs() {
    int charWidthLoc = hg::getLocationFromName(&asciiBandInfo, "CharWidthNorm"), charHeightLoc = hg::getLocationFromName(&asciiBandInfo, "CharHeightNorm");
    
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            asciiBandCharUVs[i][j] *= glm::vec2(asciiBandInfo[charWidthLoc].getDoubleValue(), asciiBandInfo[charHeightLoc].getDoubleValue());
        }
    }
}

UIText::UIText(std::string text, Shader *shader, const RenderData *data):
shader(shader), data(data), asciiBandTexture("resources/font/asciiBand.png") {
    
//    if(asciiBandCharUVs[0][1][0] == 1.0f) {
        prepareAsciiBandUVs();
//    }
    
    int xOffsetLoc = hg::getLocationFromName(&asciiBandInfo, "XOffsetNorm"), yOffsetLoc = hg::getLocationFromName(&asciiBandInfo, "YOffsetNorm"), charWidthNormLoc = hg::getLocationFromName(&asciiBandInfo, "CharWidthNorm"),  charWidthLoc = hg::getLocationFromName(&asciiBandInfo, "CharWidth"), charHeightLoc = hg::getLocationFromName(&asciiBandInfo, "CharHeight");
    
    for(int i = 0; i < text.size(); i++) {
        if(text[i] >= 0 && text[i] <= 127) {
            chars.push_back(std::make_unique<UIRectangle>(shader, data, &asciiBandTexture, asciiBandCharUVs));
            chars[chars.size() - 1]->setTextureOffset(glm::vec2(float(text[i] - 32) * asciiBandInfo[charWidthNormLoc].getDoubleValue(), 0.0f));
            chars[chars.size() - 1]->setScale(glm::vec3(asciiBandInfo[charWidthLoc].getIntValue(), asciiBandInfo[charHeightLoc].getIntValue(), 0.0f));
            chars[chars.size() - 1]->setTranslation(glm::vec3(float(i * asciiBandInfo[charWidthLoc].getIntValue()), 0.0f, 0.0f));
        }
        else {
            printf("Non ascii characters are not supported yet!\n");
        }
    }
    
    for(int i = 0; i < chars.size(); i++) {
        chars[i]->addToTriangleList(&triangles);
    }
}

UIText::~UIText() {
    
}

void UIText::render() {
    shader->use();
    
    for(int i = 0; i < triangles.size(); i++) {
        triangles[i]->render();
    }
}

void UIText::calculateModelMat() {
    for(int i = 0; i < chars.size(); i++) {
        chars[i]->setModelMat(modelMat);
    }
}

