//
//  texture.hpp
//  Lesson 5 - Textures
//
//  Created by Nathan on 2019/1/23.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <GL/glew.h>

#include "stb_image.h"

class Texture {
public:
    Texture();
    Texture(char* file_loc);

    void LoadTexture();
    void UseTexture();
    void ClearTexture();

    ~Texture();
private:
    GLuint texture_id;
    int width, height, bit_depth;
    const char* file_path;
};

#endif /* _TEXTURE_HPP_ */
