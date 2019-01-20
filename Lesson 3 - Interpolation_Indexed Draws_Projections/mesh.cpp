//
//  Mesh.cpp
//  Lesson 3 - Interpolation_Indexed Draws_Projections
//
//  Created by Nathan on 2019/1/20.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "mesh.hpp"

Mesh::Mesh() {
    vao = 0;
    vbo = 0;
    ibo = 0;
    index_count = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices) {
    index_count = num_of_indices;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * num_of_indices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * num_of_vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  0);
    return;
}

int Mesh::RenderMesh(Shader* shader){
    glBindVertexArray(vao);
    if(!shader->ValidateProgram()) return 0;
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return 1;
}

void Mesh::ClearMesh(){
    if (ibo != 0) {
        glDeleteBuffers(1, &ibo);
        ibo = 0;
    }
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
        ibo = 0;
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    index_count = 0;
    return;
}

Mesh::~Mesh(){
    ClearMesh();
}
