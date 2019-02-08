//
//  material.hpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/24.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include <stdio.h>

#include <GL/glew.h>

class Material {

public:
    Material();
    Material(GLfloat s_intensity, GLfloat shine);

    void UseMaterial(GLuint specular_intensity_location, GLuint shininess_location);

    ~Material();
private:
    GLfloat specular_intensity;
    GLfloat shininess;
};

#endif /* _MATERIAL_HPP_ */
