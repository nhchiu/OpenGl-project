//
//  spotlight.cpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/25.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "spot_light.hpp"

SpotLight::SpotLight(): PointLight() {
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0.0f;
    cos_edge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat a_intensity, GLfloat d_intensity,
                     GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
                     GLfloat x_dir, GLfloat y_dir, GLfloat z_dir,
                     GLfloat con, GLfloat lin, GLfloat exp,
                     GLfloat edg):
PointLight(red, green, blue,
           a_intensity, d_intensity,
           x_pos, y_pos, z_pos,
           con, lin, exp) {
    direction = glm::normalize(glm::vec3(x_dir, y_dir, z_dir));
    edge = edg;
    cos_edge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambient_intensity_location, GLuint color_location,
                         GLuint diffuse_intensity_location, GLuint position_location, GLuint direction_location,
                         GLuint constant_location, GLuint linear_location, GLuint exponent_location,
                         GLuint edge_location) {
    glUniform3f(color_location, color.x, color.y, color.z);
    glUniform1f(ambient_intensity_location, ambient_intensity);
    glUniform1f(diffuse_intensity_location, diffuse_intensity);

    glUniform3f(position_location, position.x, position.y, position.z);

    glUniform1f(constant_location, constant);
    glUniform1f(linear_location, linear);
    glUniform1f(exponent_location, exponent);

    glUniform3f(direction_location, direction.x, direction.y, direction.z);
    glUniform1f(edge_location, cos_edge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir) {
    position = pos;
    direction = dir;
}

SpotLight::~SpotLight() {
}
