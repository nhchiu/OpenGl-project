//
//  camera.cpp
//  Lesson 5 - Textures
//
//  Created by Nathan on 2019/1/22.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "camera.hpp"

Camera::Camera() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    move_speed = 2.0f;
    turn_speed = 0.2f;

    update();
}

Camera::Camera(glm::vec3 start_position, glm::vec3 start_up, GLfloat start_yaw, GLfloat start_pitch, GLfloat start_move_speed, GLfloat start_turn_speed) {
    position = start_position;
    world_up = start_up;
    yaw = start_yaw;
    pitch = start_pitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    move_speed = start_move_speed;
    turn_speed = start_turn_speed;

    update();
}

void Camera::KeyControl(bool* keys, GLfloat delta_time) {
    GLfloat displacement = move_speed * delta_time;

    if (keys[GLFW_KEY_UP])
        position += front * displacement;
    if (keys[GLFW_KEY_DOWN])
        position -= front * displacement;

    if (keys[GLFW_KEY_LEFT])
        position -= right * displacement;
    if (keys[GLFW_KEY_RIGHT])
        position += right * displacement;
}

void Camera::MouseControl(GLfloat x_move, GLfloat y_move) {
    yaw += x_move * turn_speed;
    pitch += y_move * turn_speed;

    if (pitch >= 89.0f)
        pitch = 89.0f;
    else if (pitch <= -89.0f)
        pitch = -89.0f;

    update();
}

void Camera::update() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {
}
