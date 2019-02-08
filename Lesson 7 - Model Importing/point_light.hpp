//
//  directional_light.hpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/25.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "light.hpp"

class PointLight : public Light{
public:
    PointLight();

    PointLight(GLfloat red, GLfloat green, GLfloat blue,
               GLfloat a_intensity, GLfloat d_intensity,
               GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
               GLfloat con, GLfloat lin, GLfloat exp);

    void UseLight(GLuint ambient_intensity_location, GLuint color_location,
                  GLuint diffuse_intensity_location, GLuint position_location,
                  GLuint constant_location, GLuint linear_location, GLuint exponent_location);

    ~PointLight();
protected:
    glm::vec3 position;

    // atenuation coefficient
    GLfloat constant, linear, exponent;
};

#endif /* _POINT_LIGHT_H_ */
