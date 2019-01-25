//
//  main.cpp
//  Lesson 6 - Phong Lighting
//
//  Created by Nathan on 2019/1/24.
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
#include "texture.hpp"
#include "directional_light.hpp"
#include "point_light.hpp"
#include "spot_light.hpp"
#include "material.hpp"

#include "constants.hpp"

// Window dimensions
const GLint WIDTH = 1366, HEIGHT = 768;

Window main_window(WIDTH, HEIGHT);
std::vector<Mesh*> mesh_list;
std::vector<Shader*> shader_list;
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f),
              glm::vec3(0.0f, 1.0f, 0.0f),
              -90.0f, 0.0f,
              2.0f, 0.2f);

Texture brick_texture;
Texture dirt_texture;
Texture plain_texture;

DirectionalLight main_light;
PointLight point_lights[MAX_POINT_LIGHTS];
SpotLight spot_lights[MAX_SPOT_LIGHTS];

Material shiny_material;
Material dull_material;

GLfloat delta_time = 0.0f;
GLfloat last_time = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CalcAverageNormals(unsigned int* indices, unsigned int indices_count,
                        GLfloat *vertices, unsigned int vertices_count,
                        unsigned int v_stride, unsigned int normal_offset) {
    for (size_t i = 0; i < indices_count; i += 3) {
        unsigned int in0 = indices[i] * v_stride;
        unsigned int in1 = indices[i + 1] * v_stride;
        unsigned int in2 = indices[i + 2] * v_stride;

        glm::vec3 v1(vertices[in1] - vertices[in0],
                     vertices[in1 + 1] - vertices[in0 + 1],
                     vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0],
                     vertices[in2 + 1] - vertices[in0 + 1],
                     vertices[in2 + 2] - vertices[in0 + 2]);

        glm::vec3 normal = glm::normalize(glm::cross(v2, v1));

        in0 += normal_offset; in1 += normal_offset; in2 += normal_offset;

        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < vertices_count / v_stride; i++) {
        unsigned int n_offset = (unsigned int)(i * v_stride + normal_offset);
        glm::vec3 vec(vertices[n_offset], vertices[n_offset + 1], vertices[n_offset + 2]);
        vec = glm::normalize(vec);
        vertices[n_offset] = vec.x;
        vertices[n_offset + 1] = vec.y;
        vertices[n_offset + 2] = vec.z;
    }
}

void CreateObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
    //  x,     y,     z,       u,    v,      normal
        -1.0f, -1.0f, -0.6f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.0f,  -1.0f, 1.0f,    0.5f, 0.0f,   0.0f, 0.0f, 0.0f,
        1.0f,  -1.0f, -0.6f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.0f,  1.0f,  0.0f,    0.5f, 1.0f,   0.0f, 0.0f, 0.0f
    };

    unsigned int floor_indices[] = {
        0, 2, 1,
        1, 2, 3,
    };

    GLfloat floor_vertices[] = {
        //  x,     y,     z,    u,     v,      normal
        -10.0f, 0.0f, -10.0f,   0.0f,  0.0f,   0.0f, 1.0f, 0.0f,
        10.0f,  0.0f, -10.0f,   10.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,    0.0f,  10.0f,  0.0f, 1.0f, 0.0f,
        10.0f,  0.0f, 10.0f,    10.0f, 10.0f,  0.0f, 1.0f, 0.0f
    };

    CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    mesh_list.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    mesh_list.push_back(obj2);

    Mesh *floor = new Mesh();
    floor->CreateMesh(floor_vertices, floor_indices, 32, 6);
    mesh_list.push_back(floor);
}

void CreateShaders() {
    Shader *shader = new Shader();
    shader->CreateFromFiles(vShader, fShader);
    shader_list.push_back(shader);
}

int main(int argc, const char * argv[]) {
    main_window.Initialize();

    CreateObjects();
    CreateShaders();

    brick_texture = Texture("Textures/brick.png");
    brick_texture.LoadTexture();
    dirt_texture = Texture("Textures/dirt.png");
    dirt_texture.LoadTexture();
    plain_texture = Texture("Textures/plain.png");
    plain_texture.LoadTexture();

    shiny_material = Material(1.0f, 32);
    dull_material = Material(0.3f, 4);

    main_light = DirectionalLight(1.0f, 1.0f, 1.0f,
                                  0.2f, 0.4f,
                                  2.0f, -1.0f, 2.0f);

    unsigned int point_light_count = 2;
    point_lights[0] = (PointLight(0.0f, 0.0f, 1.0f,
                                  0.1f, 0.3f,
                                  -4.0f, 0.0f, 0.0f,
                                  0.3f, 0.2f, 0.1f));
    point_lights[1] = (PointLight(0.0f, 1.0f, 0.0f,
                                  0.1f, 0.3f,
                                  4.0f, -1.0f, 0.0f,
                                  0.3f, 0.2f, 0.1f));

    unsigned int spot_light_count = 2;
    spot_lights[0] = (SpotLight(1.0f, 1.0f, 1.0f,
                                0.1f, 0.5f,
                                0.0f, 0.0f, 0.0f,
                                0.0f, -1.0f, 0.0f,
                                0.3f, 0.2f, 0.1f,
                                20.0f));
    spot_lights[1] = (SpotLight(1.0f, 0.5f, 0.5f,
                                0.1f, 0.5f,
                                2.0f, -1.0f, 0.0f,
                                -1.0f, -1.0f, 0.0f,
                                1.0f, 0.1f, 0.1f,
                                25.0f));

    GLuint u_projection = 0, u_model = 0, u_view = 0, u_eye_position = 0;
    GLuint u_specular_intensity = 0, u_shininess = 0;

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

        u_eye_position = shader_list[0]->GetEyePositionLocation();

        u_specular_intensity = shader_list[0]->GetSpecularIntensityLocation();
        u_shininess = shader_list[0]->GetShininessLocation();

        spot_lights[0].SetFlash(camera.GetCameraPosition() + glm::vec3(0.0f, -0.5f, 0.0f), camera.GetCameraDirection());

        shader_list[0]->SetDirectionalLight(&main_light);
        shader_list[0]->SetPointLight(point_lights, point_light_count);
        shader_list[0]->SetSpotLight(spot_lights, spot_light_count);

        glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        glUniform3f(u_eye_position, camera.GetCameraPosition().x,
                                    camera.GetCameraPosition().y,
                                    camera.GetCameraPosition().z);

        glm::mat4 model;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));
        brick_texture.UseTexture();
        shiny_material.UseMaterial(u_specular_intensity, u_shininess);
        if(!mesh_list[0]->RenderMesh(shader_list[0]))
            break;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, -1.0f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));
        dirt_texture.UseTexture();
        dull_material.UseMaterial(u_specular_intensity, u_shininess);
        if(!mesh_list[1]->RenderMesh(shader_list[0]))
            break;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f, 10.0f, 1.0f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));
        plain_texture.UseTexture();
        dull_material.UseMaterial(u_specular_intensity, u_shininess);
        if(!mesh_list[2]->RenderMesh(shader_list[0]))
        break;

        glUseProgram(0);

        main_window.SwapBuffer();
    }

    glfwTerminate();

    return 0;
}
