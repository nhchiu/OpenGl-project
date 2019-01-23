#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec4 vertex_color;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vertex_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    tex_coord = tex;
}
