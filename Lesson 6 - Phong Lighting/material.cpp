//
//  material.cpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/24.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "material.hpp"

Material::Material() {
    specular_intensity = 0.0f;
    shininess = 0.0f;
}

Material::Material(GLfloat s_intensity, GLfloat shine) {
    specular_intensity = s_intensity;
    shininess = shine;
}

void Material::UseMaterial(GLuint specular_intensity_location, GLuint shininess_location) {
    glUniform1f(specular_intensity_location, specular_intensity);
    glUniform1f(shininess_location, shininess);
}

Material::~Material() {
}
