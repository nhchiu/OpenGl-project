//
//  directional_light.cpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/25.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "point_light.hpp"

PointLight::PointLight(): Light() {
    position = glm::vec3(5.0f, 5.0f, 5.0f);
    constant = 1.0f;
    linear = 0.2f;
    exponent = 0.2f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat a_intensity, GLfloat d_intensity,
                       GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
                       GLfloat con, GLfloat lin, GLfloat exp):
Light(red, green, blue, a_intensity, d_intensity) {
    position = glm::vec3(x_pos, y_pos, z_pos);
    constant = con;
    linear = lin;
    exponent = exp;
}

void PointLight::UseLight(GLuint ambient_intensity_location, GLuint color_location,
                          GLuint diffuse_intensity_location, GLuint position_location,
                          GLuint constant_location, GLuint linear_location, GLuint exponent_location) {
    glUniform3f(color_location, color.x, color.y, color.z);
    glUniform1f(ambient_intensity_location, ambient_intensity);
    glUniform1f(diffuse_intensity_location, diffuse_intensity);

    glUniform3f(position_location, position.x, position.y, position.z);

    glUniform1f(constant_location, constant);
    glUniform1f(linear_location, linear);
    glUniform1f(exponent_location, exponent);
}

PointLight::~PointLight() {
}
