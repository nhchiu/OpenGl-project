//
//  light.hpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/24.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include <stdio.h>

#include <GL/glew.h>

#include <glm/glm.hpp>

class Light {

public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue,
          GLfloat a_intensity, GLfloat d_intensity);

    ~Light();

protected:
    glm::vec3 color;
    GLfloat ambient_intensity;
    GLfloat diffuse_intensity;
};

#endif /* _LIGHT_HPP_ */
