//
//  Mesh.hpp
//  Lesson 5 - Textures
//
//  Created by Nathan on 2019/1/20.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "shader.hpp"

#include <stdio.h>

#include <GL/glew.h>

class Mesh {
public:
    Mesh();

    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices);
    int RenderMesh(Shader* shader);
    void ClearMesh();

    ~Mesh();
private:
    GLuint vao, vbo, ibo;
    GLsizei index_count;
};

#endif /* _MESH_HPP_ */
