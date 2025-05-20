#pragma once

#include <stdio.h>

#include <stb_image.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "core.h"

class TextureManager {
private:

    std::vector<GLuint> textures;

public:

    /*
    Initializes TextureManager that will hold all textures
    */
    TextureManager();
    /*
    Deletes the TextureManager and all associated textures
    */
    ~TextureManager();

    GLuint LoadTexture(const char* texture_file_path);

    GLuint getTexture(int id);
};