//
//  circle.cpp
//  openglEarthSatellite
//
//  Created by Wang Bin on 2021/7/10.
//

#include "circle.h"

/*
Circle::Circle(glm::vec3 _center, float _radius, glm::vec3 _color, unsigned int _num_vertices)
: Center(_center), Radius(_radius), Color(_color), shader(vertex_shader_circle, fragment_shader_circle, true)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Create vertices of a 2d circle line
    for (float angle = 0.0f; angle <= 2.0f * M_PI; angle += 2.0f * M_PI / _num_vertices)
    {
        Vertex v;
        v.Position = glm::vec3(Radius * cos(angle) + Center.x, Radius * sin(angle) + Center.y, 0);
        v.Color = Color;
        vertices.push_back(v);
    }

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, Color)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Circle::Draw()
{
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
    glBindVertexArray(0);
}
*/

