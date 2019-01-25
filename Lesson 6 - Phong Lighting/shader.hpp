//
//  Shader.hpp
//  Lesson 5 - Textures
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
#include <vector>

#include <GL/glew.h>

#include "directional_light.hpp"
#include "point_light.hpp"
#include "spot_light.hpp"

#include "constants.hpp"

class Shader {
public:
    Shader();

    void CreateFromString(const char* vertex_shader_code, const char* fragment_shader_code);
    void CreateFromFiles(const char* vertex_shader_file, const char* fragment_shader_file);

    std::string ReadFile(const char* shader_file);

    GLuint GetProjectionLocation() { return u_projection; }
    GLuint GetModelLocation() { return u_model; }
    GLuint GetViewLocation() { return u_view; }
    GLuint GetEyePositionLocation() { return u_eye_position; }
    // GLuint GetColorLocation() { return u_directional_light.u_color; }
    // GLuint GetAmbientIntensityLocation() { return u_directional_light.u_ambient_intensity; }
    // GLuint GetDiffuseIntensityLocation() { return u_directional_light.u_diffuse_intensity; }
    // GLuint GetDirectionLocation() { return u_directional_light.u_direction; }
    GLuint GetSpecularIntensityLocation() { return u_specular_intensity; }
    GLuint GetShininessLocation() { return u_shininess; }

    int GetPointLightCount() { return point_light_count; }
    int GetSpotLightCount() { return spot_light_count; }

    int AddPointLight(GLfloat red, GLfloat green, GLfloat blue,
                      GLfloat a_intensity, GLfloat d_intensity,
                      GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
                      GLfloat con, GLfloat lin, GLfloat exp);
    int AddSpotLight(GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat a_intensity, GLfloat d_intensity,
                     GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
                     GLfloat x_dir, GLfloat y_dir, GLfloat z_dir,
                     GLfloat con, GLfloat lin, GLfloat exp,
                     GLfloat edg);

    void SetDirectionalLight(DirectionalLight* d_light);
    void SetPointLight(PointLight* p_light, unsigned int light_count);
    void SetSpotLight(SpotLight* s_light, unsigned int light_count);

    void UseShader();
    void ClearShader();

    GLint ValidateProgram();

    ~Shader();

private:
    int point_light_count;
    int spot_light_count;

    std::vector<PointLight*> point_lights;
    std::vector<SpotLight*> spot_lights;

    GLuint u_point_light_count;
    GLuint u_spot_light_count;

    GLuint shader_id;
    GLuint u_projection, u_model, u_view, u_eye_position;
    GLuint u_specular_intensity, u_shininess;

    struct {
        GLuint u_color;
        GLuint u_ambient_intensity;
        GLuint u_diffuse_intensity;
        GLuint u_direction;
    } u_directional_light;

    struct {
        GLuint u_color;
        GLuint u_ambient_intensity;
        GLuint u_diffuse_intensity;
        GLuint u_position;
        GLuint u_constant;
        GLuint u_linear;
        GLuint u_exponent;
    } u_point_light[MAX_POINT_LIGHTS];

    struct {
        GLuint u_color;
        GLuint u_ambient_intensity;
        GLuint u_diffuse_intensity;
        GLuint u_position;
        GLuint u_constant;
        GLuint u_linear;
        GLuint u_exponent;
        GLuint u_direction;
        GLuint u_edge;
    } u_spot_light[MAX_SPOT_LIGHTS];

    void CompileShader(const char* vertex_shader_code, const char* fragment_shader_code);
    void AddShader(GLuint the_program, const char* shader_code, GLenum shader_type);
};

#endif /* _SHADER_HPP_ */
