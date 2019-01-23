//
//  Shader.cpp
//  Lesson 5 - Textures
//
//  Created by Nathan on 2019/1/20.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "shader.hpp"

Shader::Shader(){
    shader_id = 0;
    u_projection = 0;
    u_model = 0;
}

void Shader::CreateFromString(const char *vertex_shader_code, const char *fragment_shader_code) {
    CompileShader(vertex_shader_code, fragment_shader_code);
    return;
}

void Shader::CreateFromFiles(const char* vertex_shader_file, const char* fragment_shader_file) {
    std::string vertex_shader_string = ReadFile(vertex_shader_file);
    std::string fragment_shader_string = ReadFile(fragment_shader_file);
    const char* vertex_shader_code = vertex_shader_string.c_str();
    const char* fragment_shader_code = fragment_shader_string.c_str();

    CompileShader(vertex_shader_code, fragment_shader_code);
    return;
}

std::string Shader::ReadFile(const char* shader_file) {
    std::string code = "";
    std::ifstream file_stream(shader_file, std::ifstream::in);

    if (!file_stream.is_open()) {
        printf("Failed to read \"%s\"! File doesn't exist.\n", shader_file);
        return code;
    }

    std::string line = "";
    while (!file_stream.eof()) {
        std::getline(file_stream, line);
        code.append(line + "\n");
    }

    file_stream.close();
    return code;
}

GLuint Shader::GetProjectionLocation() {
    return u_projection;
}

GLuint Shader::GetModelLocation() {
    return u_model;
}

GLuint Shader::GetViewLocation() {
    return u_view;
}

void Shader::UseShader() {
    glUseProgram(shader_id);
    return;
}

void Shader::ClearShader() {
    if (shader_id != 0) {
        glDeleteProgram(shader_id);
        shader_id = 0;
    }
    u_projection = 0;
    u_model = 0;
    return;
}

GLint Shader::ValidateProgram(){
    GLint result = 0;
    glValidateProgram(shader_id);
    glGetProgramiv(shader_id, GL_VALIDATE_STATUS, &result);
    if(!result) {
        GLchar e_log[1024] = {0};
        glGetProgramInfoLog(shader_id, sizeof(e_log), NULL, e_log);
        printf("Error validating program: '%s'\n", e_log);
    }
    return result;
}

void Shader::CompileShader(const char* vertex_shader_code, const char* fragment_shader_code) {
    shader_id = glCreateProgram();

    if(!shader_id) {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shader_id, vertex_shader_code, GL_VERTEX_SHADER);
    AddShader(shader_id, fragment_shader_code, GL_FRAGMENT_SHADER);

    GLint result = 0;

    glLinkProgram(shader_id);
    glGetProgramiv(shader_id, GL_LINK_STATUS, &result);
    if(!result) {
        GLchar eLog[1024] = {0};
        glGetProgramInfoLog(shader_id, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    u_model = glGetUniformLocation(shader_id, "model");
    u_projection = glGetUniformLocation(shader_id, "projection");
    u_view = glGetUniformLocation(shader_id, "view");
    return;
}

void Shader::AddShader(GLuint the_program, const char* shader_code, GLenum shader_type) {
    GLuint the_shader = glCreateShader(shader_type);

    const GLchar* the_code[1];
    the_code[0] = shader_code;

    GLint code_length[1];
    code_length[0] = (GLint)strlen(shader_code);

    glShaderSource(the_shader, 1, the_code, code_length);
    glCompileShader(the_shader);

    GLint result = 0;
    GLchar eLog[1024] = {0};
 
    glGetShaderiv(the_shader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(the_shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shader_type, eLog);
        return;
    }

    glAttachShader(the_program, the_shader);

    return;
}

Shader::~Shader() {
    ClearShader();
}
