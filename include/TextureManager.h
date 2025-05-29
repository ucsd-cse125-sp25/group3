#pragma once

#include <stdio.h>

#include <stb_image.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core.h"

class TextureManager {
private:

    // std::vector<GLuint> textures;
    std::map<std::string, GLuint> textures;

public:

    /*
    Initializes TextureManager that will hold all textures
    */
    TextureManager();
    /*
    Deletes the TextureManager and all associated textures
    */
    ~TextureManager();

    GLuint LoadTexture(std::string texture_file_path);
};