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

    point_light_count = 0;
    spot_light_count = 0;
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

int Shader::AddPointLight(GLfloat red, GLfloat green, GLfloat blue,
                          GLfloat a_intensity, GLfloat d_intensity,
                          GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
                          GLfloat con, GLfloat lin, GLfloat exp) {
    if (point_light_count >= MAX_POINT_LIGHTS) {
        printf("Reach maximum number: %d of point lights!", MAX_POINT_LIGHTS);
        return 0;
    }
    point_light_count++;
    PointLight* point_light = new PointLight(red, green, blue,
                                             a_intensity, d_intensity,
                                             x_pos, y_pos, z_pos,
                                             con, lin, exp);
    point_lights.push_back(point_light);
    return 1;
}

int Shader::AddSpotLight(GLfloat red, GLfloat green, GLfloat blue,
                         GLfloat a_intensity, GLfloat d_intensity,
                         GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
                         GLfloat x_dir, GLfloat y_dir, GLfloat z_dir,
                         GLfloat con, GLfloat lin, GLfloat exp,
                         GLfloat edg) {
    if (spot_light_count >= MAX_SPOT_LIGHTS) {
        printf("Reach maximum number: %d of spot lights!", MAX_SPOT_LIGHTS);
        return 0;
    }
    spot_light_count++;
    SpotLight* spot_light = new SpotLight(red, green, blue,
                                          a_intensity, d_intensity,
                                          x_pos, y_pos, z_pos,
                                          x_dir, y_dir, z_dir,
                                          con, lin, exp,
                                          edg);
    spot_lights.push_back(spot_light);
    return 1;
}

void Shader::SetDirectionalLight(DirectionalLight* d_light) {
    d_light->UseLight(u_directional_light.u_ambient_intensity,
                      u_directional_light.u_color,
                      u_directional_light.u_diffuse_intensity,
                      u_directional_light.u_direction);
}

void Shader::SetPointLight(PointLight* p_light, unsigned int light_count) {
    if (light_count > MAX_POINT_LIGHTS) light_count = MAX_POINT_LIGHTS;

    glUniform1i(u_point_light_count, light_count);

    for (int i = 0; i < light_count; i++) {
        p_light[i].UseLight(u_point_light[i].u_ambient_intensity,
                            u_point_light[i].u_color,
                            u_point_light[i].u_diffuse_intensity,
                            u_point_light[i].u_position,
                            u_point_light[i].u_constant,
                            u_point_light[i].u_linear,
                            u_point_light[i].u_exponent);
    }
}

void Shader::SetSpotLight(SpotLight* s_light, unsigned int light_count) {
    if (light_count > MAX_SPOT_LIGHTS) light_count = MAX_SPOT_LIGHTS;

    glUniform1i(u_spot_light_count, light_count);

    for (int i = 0; i < light_count; i++) {
        s_light[i].UseLight(u_spot_light[i].u_ambient_intensity,
                            u_spot_light[i].u_color,
                            u_spot_light[i].u_diffuse_intensity,
                            u_spot_light[i].u_position,
                            u_spot_light[i].u_direction,
                            u_spot_light[i].u_constant,
                            u_spot_light[i].u_linear,
                            u_spot_light[i].u_exponent,
                            u_spot_light[i].u_edge);
    }

}

void Shader::UseShader() {
    glUseProgram(shader_id);
}

void Shader::ClearShader() {
    if (shader_id != 0) {
        glDeleteProgram(shader_id);
        shader_id = 0;
    }
    u_projection = 0;
    u_model = 0;

    for (int i = 0; i < point_light_count; i++) {
        delete point_lights.at(i);
    }
    for (int i = 0; i < spot_light_count; i++) {
        delete spot_lights.at(i);
    }
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

    u_eye_position = glGetUniformLocation(shader_id, "eye_position");

    u_specular_intensity = glGetUniformLocation(shader_id, "material.specular_intensity");
    u_shininess = glGetUniformLocation(shader_id, "material.shininess");

    u_directional_light.u_ambient_intensity = glGetUniformLocation(shader_id, "directional_light.base.ambient_intensity");
    u_directional_light.u_diffuse_intensity = glGetUniformLocation(shader_id, "directional_light.base.diffuse_intensity");
    u_directional_light.u_color = glGetUniformLocation(shader_id, "directional_light.base.color");
    u_directional_light.u_direction = glGetUniformLocation(shader_id, "directional_light.direction");

    u_point_light_count = glGetUniformLocation(shader_id, "point_light_count");

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        char buff[128] = {'\0'};

        snprintf(buff, sizeof(buff), "point_lights[%d].base.ambient_intensity", i);
        u_point_light[i].u_ambient_intensity = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "point_lights[%d].base.diffuse_intensity", i);
        u_point_light[i].u_diffuse_intensity = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "point_lights[%d].base.color", i);
        u_point_light[i].u_color = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "point_lights[%d].position", i);
        u_point_light[i].u_position = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "point_lights[%d].constant", i);
        u_point_light[i].u_constant = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "point_lights[%d].linear", i);
        u_point_light[i].u_linear = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "point_lights[%d].exponent", i);
        u_point_light[i].u_exponent = glGetUniformLocation(shader_id, buff);
    }

    u_spot_light_count = glGetUniformLocation(shader_id, "spot_light_count");

    for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char buff[128] = {'\0'};

        snprintf(buff, sizeof(buff), "spot_lights[%d].base.base.ambient_intensity", i);
        u_spot_light[i].u_ambient_intensity = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "spot_lights[%d].base.base.diffuse_intensity", i);
        u_spot_light[i].u_diffuse_intensity = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "spot_lights[%d].base.base.color", i);
        u_spot_light[i].u_color = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "spot_lights[%d].base.position", i);
        u_spot_light[i].u_position = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "spot_lights[%d].base.constant", i);
        u_spot_light[i].u_constant = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "spot_lights[%d].base.linear", i);
        u_spot_light[i].u_linear = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "spot_lights[%d].base.exponent", i);
        u_spot_light[i].u_exponent = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "spot_lights[%d].direction", i);
        u_spot_light[i].u_direction = glGetUniformLocation(shader_id, buff);

        snprintf(buff, sizeof(buff), "spot_lights[%d].edge", i);
        u_spot_light[i].u_edge = glGetUniformLocation(shader_id, buff);
    }
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
}

Shader::~Shader() {
    ClearShader();
}
