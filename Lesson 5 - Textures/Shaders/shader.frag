#version 410

in vec4 vertex_color;
in vec2 tex_coord;

out vec4 color;

uniform sampler2D the_texture;

void main(){
    color = texture(the_texture, tex_coord);
}
