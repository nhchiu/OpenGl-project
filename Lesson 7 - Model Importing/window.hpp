//
//  window.hpp
//  Lesson 5 - Textures
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
    GLfloat GetXChange();
    GLfloat GetYChange();

    bool* GetKeys() { return keys; }

    void SwapBuffer() { glfwSwapBuffers(main_window); return; }

    ~Window();
private:
    GLFWwindow *main_window;
    GLint width, height;
    GLint buffer_width, buffer_height;

    bool keys[1024];

    GLfloat last_x, last_y;
    GLfloat x_change, y_change;
    bool mouse_first_moved = false;

    void CreateCallbacks();

    static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);
    static void HandleMouse(GLFWwindow *window, double x_pos, double y_pos);
};

#endif /* _WINDOW_HPP_ */
