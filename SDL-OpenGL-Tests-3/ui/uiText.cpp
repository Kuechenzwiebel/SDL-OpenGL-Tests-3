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

static int xOffsetLoc = hg::getLocationFromName(&asciiBandInfo, "XOffsetNorm"), yOffsetLoc = hg::getLocationFromName(&asciiBandInfo, "YOffsetNorm"), charWidthNormLoc = hg::getLocationFromName(&asciiBandInfo, "CharWidthNorm"),  charHeightNormLoc = hg::getLocationFromName(&asciiBandInfo, "CharHeightNorm"),  charWidthLoc = hg::getLocationFromName(&asciiBandInfo, "CharWidth"), charHeightLoc = hg::getLocationFromName(&asciiBandInfo, "CharHeight");

static void prepareAsciiBandUVs() {
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            asciiBandCharUVs[i][j] *= glm::vec2(asciiBandInfo[charWidthNormLoc].getDoubleValue(), asciiBandInfo[charHeightNormLoc].getDoubleValue());
        }
    }
}

UIText::UIText(std::string text, Shader *shader, const RenderData *data):
shader(shader), data(data), asciiBandTexture("resources/font/asciiBand.png") {
    if(asciiBandCharUVs[0][1][0] == 1.0f) {
        prepareAsciiBandUVs();
    }
    
    scale = glm::vec3(asciiBandInfo[charWidthLoc].getIntValue(), asciiBandInfo[charHeightLoc].getIntValue(), 0.0f);
    
    setText(text);
}

UIText::~UIText() {
    
}

void UIText::setText(std::string newText) {
    if(newText == text) {
        text = newText;
        return;
    }
    
    chars.clear();
    triangles.clear();
    
    unsigned int rows = 0, cols = 0;
    
    for(int i = 0; i < newText.size(); i++) {
        if(newText[i] == '\n') {
            rows++;
            cols = 0;
            continue;
        }
        
        if(newText[i] >= 0 && newText[i] <= 127) {
            chars.push_back(std::make_pair(std::make_pair(cols, rows), std::make_unique<UIRectangle>(shader, data, &asciiBandTexture, asciiBandCharUVs)));
            chars[chars.size() - 1].second->setTextureOffset(glm::vec2(float(newText[i] - 32) * asciiBandInfo[charWidthNormLoc].getDoubleValue() + asciiBandInfo[xOffsetLoc].getDoubleValue(), asciiBandInfo[yOffsetLoc].getDoubleValue()));
            chars[chars.size() - 1].second->setScale(glm::vec3(asciiBandInfo[charWidthLoc].getIntValue(), asciiBandInfo[charHeightLoc].getIntValue(), 0.0f));
            chars[chars.size() - 1].second->setTranslation(glm::vec3(float(cols * asciiBandInfo[charWidthLoc].getIntValue()), -float(rows * asciiBandInfo[charHeightLoc].getIntValue()), 0.0f));
        }
        else {
            printf("Non ascii characters are not supported yet!\n");
        }
        
        cols++;
    }
    
    for(int i = 0; i < chars.size(); i++) {
        chars[i].second->addToTriangleList(&triangles);
    }
}

void UIText::render() {
    shader->use();
    
    for(int i = 0; i < triangles.size(); i++) {
        triangles[i]->render();
    }
}

void UIText::calculateModelMat() {
    for(int i = 0; i < chars.size(); i++) {
        chars[i].second->setScale(scale);
        chars[i].second->setRotation(rotation);
        chars[i].second->setTranslation(translation + glm::vec3(float(chars[i].first.first), -float(chars[i].first.second), 0.0f) * chars[i].second->getScale());
    }
}

