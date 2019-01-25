//
//  light.cpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/24.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "light.hpp"

Light::Light() {
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambient_intensity = 1.0f;
    diffuse_intensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue,
             GLfloat a_intensity, GLfloat d_intensity) {
    color = glm::vec3(glm::clamp(red, 0.0f, 1.0f),
                      glm::clamp(green, 0.0f, 1.0f),
                      glm::clamp(blue, 0.0f, 1.0f));
    ambient_intensity = a_intensity;
    diffuse_intensity = d_intensity;
}

Light::~Light() {
}
