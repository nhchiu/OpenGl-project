//
//  camera.hpp
//  Lesson 5 - Textures
//
//  Created by Nathan on 2019/1/22.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <stdio.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    Camera(glm::vec3 start_position, glm::vec3 start_up, GLfloat start_yaw, GLfloat start_pitch, GLfloat start_move_speed, GLfloat start_turn_speed);

    void KeyControl(bool* keys, GLfloat delta_time);
    void MouseControl(GLfloat x_move, GLfloat y_move);

    glm::mat4 CalculateViewMatrix();
    
    ~Camera();
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat move_speed;
    GLfloat turn_speed;

    void update();
};

#endif /* _CAMERA_HPP_ */
