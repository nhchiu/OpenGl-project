//
//  directional_light.cpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/25.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "directional_light.hpp"

DirectionalLight::DirectionalLight(): Light() {
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat a_intensity, GLfloat d_intensity,
                                   GLfloat x_dir, GLfloat y_dir, GLfloat z_dir):
Light(red, green, blue, a_intensity, d_intensity) {
    direction = glm::vec3(x_dir, y_dir, z_dir);
}

void DirectionalLight::UseLight(GLuint ambient_intensity_location, GLuint color_location,
                                GLuint diffuse_intensity_location, GLuint direction_location) {
    glUniform3f(color_location, color.x, color.y, color.z);
    glUniform1f(ambient_intensity_location, ambient_intensity);
    glUniform1f(diffuse_intensity_location, diffuse_intensity);

    glUniform3f(direction_location, direction.x, direction.y, direction.z);
}

DirectionalLight::~DirectionalLight() {
}
