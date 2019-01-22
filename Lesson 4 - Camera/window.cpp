//
//  window.cpp
//  Lesson 3 - Interpolation_Indexed Draws_Projections
//
//  Created by Nathan on 2019/1/20.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "window.hpp"

Window::Window() {
    width = 800;
    height = 600;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
}

Window::Window(GLint window_width, GLint window_height) {
    width = window_width;
    height = window_height;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
}

int Window::Initialize() {
    if(!glfwInit()){
        printf("GLFW initilaization failed!");
        glfwTerminate();
        return 0;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // Core profile = no backward compatability
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatability
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Macos only
    glfwWindowHint(GLFW_RELEASE, GL_FALSE);

    main_window = glfwCreateWindow(width, height, "OpenGL Project", NULL, NULL);

    if(main_window == NULL) {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 0;
    }

    // Get buffer size information
    glfwGetFramebufferSize(main_window, &buffer_width, &buffer_height);
    printf("Screen Width: %d\n", buffer_width);
    printf("Screen Height: %d\n", buffer_height);

    // Set context for GLFW to use
    glfwMakeContextCurrent(main_window);

    // Handle Key and Mouse input
    CreateCallbacks();
    glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK) {
        printf("GLEW initilaization failed!");
        glfwDestroyWindow(main_window);
        glfwTerminate();
        return 0;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, buffer_width, buffer_height);

    glfwSetWindowUserPointer(main_window, this);

    return 1;
}

void Window::CreateCallbacks() {
    glfwSetKeyCallback(main_window, HandleKeys);
    glfwSetCursorPosCallback(main_window, HandleMouse);
}

GLfloat Window::GetXChange() {
    GLfloat the_change = x_change;
    x_change = 0.0f;
    return the_change;
}

GLfloat Window::GetYChange() {
    GLfloat the_change = y_change;
    y_change = 0.0f;
    return the_change;
}

void Window::HandleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
    Window* the_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            the_window->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            the_window->keys[key] = false;
        }
    }
}

void Window::HandleMouse(GLFWwindow *window, double x_pos, double y_pos) {
    Window* the_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (the_window->mouse_first_moved) {
        the_window->last_x = x_pos;
        the_window->last_y = y_pos;
        the_window->mouse_first_moved = false;
    }

    the_window->x_change = x_pos - the_window->last_x;
    the_window->y_change = the_window->last_y - y_pos;

    the_window->last_x = x_pos;
    the_window->last_y = y_pos;
}

Window::~Window() {
    glfwDestroyWindow(main_window);
    glfwTerminate();
}
