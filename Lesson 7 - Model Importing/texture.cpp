//
//  texture.cpp
//  Lesson 5 - Textures
//
//  Created by Nathan on 2019/1/23.
//  Copyright © 2019 Nathan. All rights reserved.
//

#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"

Texture::Texture() {
    texture_id = 0;
    width = 0;
    height = 0;
    bit_depth = 0;
    file_path = "";
}

Texture::Texture(const char* file_loc) {
    texture_id = 0;
    width = 0;
    height = 0;
    bit_depth = 0;
    file_path = file_loc;
}

bool Texture::LoadTexture() {
    unsigned char *tex_data = stbi_load(file_path, &width, &height, &bit_depth, 0);
    if (!tex_data) {
        printf("Failed to find \"%s\"\n", file_path);
        return false;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(tex_data);
    return true;
}

bool Texture::LoadAlphaTexture() {
    unsigned char *tex_data = stbi_load(file_path, &width, &height, &bit_depth, 0);
    if (!tex_data) {
        printf("Failed to find \"%s\"\n", file_path);
        return false;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(tex_data);
    return true;
}

void Texture::UseTexture() {
    glActiveTexture(GL_TEXTURE0);  // Use texture unit 0
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::ClearTexture() {
    if (texture_id != 0) {
        glDeleteTextures(1, &texture_id);
    }
    texture_id = 0;
    width = 0;
    height = 0;
    bit_depth = 0;
    file_path = "";
}

Texture::~Texture() {
    ClearTexture();
}
