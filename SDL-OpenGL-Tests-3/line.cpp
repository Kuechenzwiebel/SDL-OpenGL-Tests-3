
//
//  line.cpp
//  SDL-OpenGL-Tests-3
//
//  Created by Tobias Pflüger on 16.02.20.
//  Copyright © 2020 Tobias Pflüger. All rights reserved.
//

#include "line.hpp"

Line::Line(Shader *shader, const RenderData *data, const glm::vec3 vertices[], const glm::vec4 colors[]):
vertex(vertices, sizeof(glm::vec3) * 2, 0), color(colors, sizeof(glm::vec4) * 2, 1), view(shader, "view", data->viewMat), projection(shader, "projection", data->projection) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertex.activate();
    color.activate();
    
    
    glBindVertexArray(0);
}

Line::~Line() {
    glDeleteVertexArrays(1, &VAO);
}

void Line::render() {
    vertex.activate();
    color.activate();
    
    projection.setVar();
    view.setVar();
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}