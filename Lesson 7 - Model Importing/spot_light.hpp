//
//  spotlight.hpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/25.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _SPOT_LIGHT_HPP_
#define _SPOT_LIGHT_HPP_

#include "point_light.hpp"

class SpotLight : public PointLight {
public:
    SpotLight();

    SpotLight(GLfloat red, GLfloat green, GLfloat blue,
              GLfloat a_intensity, GLfloat d_intensity,
              GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
              GLfloat x_dir, GLfloat y_dir, GLfloat z_dir,
              GLfloat con, GLfloat lin, GLfloat exp,
              GLfloat edg);

    void UseLight(GLuint ambient_intensity_location, GLuint color_location,
                  GLuint diffuse_intensity_location, GLuint position_location, GLuint direction_location,
                  GLuint constant_location, GLuint linear_location, GLuint exponent_location,
                  GLuint edge_location);

    void SetFlash(glm::vec3 pos, glm::vec3 dir);

    ~SpotLight();
private:
    glm::vec3 direction;

    GLfloat edge, cos_edge;
};

#endif /* _SPOT_LIGHT_HPP_ */
