//
//  main.cpp
//  Lesson 2 - Transforming
//
//  Created by Nathan on 2019/1/18.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float kToRadians = M_PI / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

bool offset_direction = true;
float offset = 0.0f;
float max_offset = 0.7f;
float offset_increment = 0.002f;

float cur_angle = 0.0f;

bool size_direction = true;
float cur_size = 0.4f;
float max_size = 0.8f;
float min_size = 0.1f;

// Vertex Shader
static const char* vShader = "                              \n\
#version 410                                                \n\
                                                            \n\
layout (location = 0) in vec3 pos;                          \n\
                                                            \n\
uniform mat4 model;                                         \n\
                                                            \n\
void main(){                                                \n\
    gl_Position = model * vec4(pos, 1.0);                   \n\
}";

// Fragment Shader
static const char* fShader = "                              \n\
#version 410                                                \n\
                                                            \n\
out vec4 color;                                             \n\
                                                            \n\
void main(){                                                \n\
    color = vec4(1.0, 0.5, 0.5, 1.0);                       \n\
}";


void CreateTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void AddShader(GLuint the_program, const char* shader_code, GLenum shader_type) {
    GLuint the_shader = glCreateShader(shader_type);

    const GLchar* the_code[1];
    the_code[0] = shader_code;

    GLint code_length[1];
    code_length[0] = strlen(shader_code);

    glShaderSource(the_shader, 1, the_code, code_length);
    glCompileShader(the_shader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(the_shader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(the_shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shader_type, eLog);
        return;
    }

    glAttachShader(the_program, the_shader);

    return;
}

void CompileShaders(){
    shader = glCreateProgram();

    if(!shader) {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
}

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
    printf("Screen Width: %d\n", bufferWidth);
    printf("Screen Height: %d\n", bufferHeight);

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

    CreateTriangle();
    CompileShaders();

    // Loop until window closed
    while(!glfwWindowShouldClose(window)) {
        // Get and Handle user input events
        glfwPollEvents();

        // Translate
        if(offset_direction) {
            offset += offset_increment;
        }
        else{
            offset -= offset_increment;
        }
        if(abs(offset) >= max_offset)
            offset_direction = !offset_direction;

        // Rotate
        cur_angle += 0.05f;
        if(cur_angle >= 360)
            cur_angle -= 360;

        // Scale
        if(size_direction) {
            cur_size += 0.0005f;
        }
        else {
            cur_size -= 0.0005f;
        }
        if(cur_size >= max_size || cur_size <= min_size)
            size_direction = !size_direction;

        // Clear window
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(offset, 0.0f, 0.0f));
        model = glm::rotate(model, cur_angle * kToRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(cur_size, cur_size, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

