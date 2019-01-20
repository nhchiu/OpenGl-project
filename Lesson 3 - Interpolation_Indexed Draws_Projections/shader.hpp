//
//  Shader.hpp
//  Lesson 3 - Interpolation_Indexed Draws_Projections
//
//  Created by Nathan on 2019/1/20.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>

class Shader {
public:
    Shader();

    void CreateFromString(const char* vertex_shader_code, const char* fragment_shader_code);
    void CreateFromFiles(const char* vertex_shader_file, const char* fragment_shader_file);

    std::string ReadFile(const char* shader_file);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();

    void UseShader();
    void ClearShader();

    GLint ValidateProgram();

    ~Shader();

private:
    GLuint shader_id, u_projection, u_model;

    void CompileShader(const char* vertex_shader_code, const char* fragment_shader_code);
    void AddShader(GLuint the_program, const char* shader_code, GLenum shader_type);
};

#endif /* Shader_hpp */
