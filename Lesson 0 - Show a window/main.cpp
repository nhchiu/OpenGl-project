//
//  main.cpp
//  OpenGl project
//
//  Created by Nathan on 2018/11/3.
//  Copyright © 2018 Nathan. All rights reserved.
//
#include <stdio.h>
#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main(int argc, const char * argv[]) {
    if(!glfwInit()){
        printf("GLFW initilaization failed!");
        glfwTerminate();
        return 1;
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
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Project", NULL, NULL);
    
    if(window == NULL) {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }
    
    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    std::cout<< "Screen Width: " << bufferWidth << std::endl;
    std::cout<< "Screen Height: " << bufferHeight << std::endl;
    
    // Set context for GLFW to use
    glfwMakeContextCurrent(window);
    
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    
    if(glewInit() != GLEW_OK) {
        printf("GLEW initilaization failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    
    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    // Loop until window closed
    while(!glfwWindowShouldClose(window)) {
        // Get and Handle user input events
        glfwPollEvents();
        
        // Clear window
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
