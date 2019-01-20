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
}

Window::Window(GLint window_width, GLint window_height) {
    width = window_width;
    height = window_height;
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

    return 1;
}

Window::~Window() {
    glfwDestroyWindow(main_window);
    glfwTerminate();
}
