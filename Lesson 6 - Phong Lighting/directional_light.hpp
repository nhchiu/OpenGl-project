//
//  directional_light.hpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/25.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "light.hpp"

class DirectionalLight : public Light{
public:
    DirectionalLight();

    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat a_intensity, GLfloat d_intensity,
                     GLfloat x_dir, GLfloat y_dir, GLfloat z_dir);

    void UseLight(GLuint ambient_intensity_location, GLuint color_location,
                  GLuint diffuse_intensity_location, GLuint direction_location);

    ~DirectionalLight();
private:
    glm::vec3 direction;
};

#endif /* _DIRECTIONAL_LIGHT_H_ */
