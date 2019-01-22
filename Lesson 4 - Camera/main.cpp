//
//  main.cpp
//  Lesson 4 - Camera
//
//  Created by Nathan on 2019/1/21.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <cmath>

#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

Window main_window(WIDTH, HEIGHT);
std::vector<Mesh*> mesh_list;
std::vector<Shader*> shader_list;
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f),
              glm::vec3(0.0f, 1.0f, 0.0f),
              -90.0f, 0.0f,
              2.0f, 0.2f);

GLfloat delta_time = 0.0f;
GLfloat last_time = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    mesh_list.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    mesh_list.push_back(obj2);
}

void CreateShaders() {
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shader_list.push_back(shader1);
}

int main(int argc, const char * argv[]) {
    main_window.Initialize();

    CreateObjects();
    CreateShaders();

    GLuint u_projection = 0, u_model = 0, u_view = 0;

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), main_window.GetAspect(), 0.1f, 100.0f);

    // Loop until window closed
    while(!main_window.ShouldClose()) {
        GLfloat current_time = glfwGetTime();  // SDL_GetPerformanceCounter();
        delta_time = current_time - last_time;  // /SDL_GetPerformanceFrequency();
        last_time = current_time;

        // Get and Handle user input events
        glfwPollEvents();

        camera.KeyControl(main_window.GetKeys(), delta_time);
        camera.MouseControl(main_window.GetXChange(), main_window.GetYChange());

        // Clear window
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_list[0]->UseShader();
        u_model = shader_list[0]->GetModelLocation();
        u_projection = shader_list[0]->GetProjectionLocation();
        u_view = shader_list[0]->GetViewLocation();

        glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection));
        
        glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(-0.7f, 0.0f, -2.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));
        if(!mesh_list[0]->RenderMesh(shader_list[0]))
            break;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.7f, 0.0f, -2.0f));
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));
        if(!mesh_list[1]->RenderMesh(shader_list[0]))
            break;

        glUseProgram(0);

        main_window.SwapBuffer();
    }

    glfwTerminate();

    return 0;
}
