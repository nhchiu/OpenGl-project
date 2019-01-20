//
//  window.hpp
//  Lesson 3 - Interpolation_Indexed Draws_Projections
//
//  Created by Nathan on 2019/1/20.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window {
public:
    Window();
    Window(GLint window_width, GLint window_height);

    int Initialize();

    GLint GetBufferWidth() { return buffer_width; }
    GLint GetBufferHeight() { return buffer_height; }
    GLfloat GetAspect() { return (GLfloat)buffer_width/(GLfloat)buffer_height; }

    bool ShouldClose() { return glfwWindowShouldClose(main_window); }

    void SwapBuffer() { glfwSwapBuffers(main_window); return; }

    ~Window();
private:
    GLFWwindow *main_window;
    GLint width, height;
    GLint buffer_width, buffer_height;
};

#endif /* window_hpp */
