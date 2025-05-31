#pragma once

#include <stdio.h>

#include <stb_image.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "core.h"

class TextureManager {
private:

    // std::vector<GLuint> textures;
    std::map<std::string, GLuint> textures;

    GLuint createTexture(unsigned char* data, int channels, int width, int height, std::string name);

public:

    /*
    Initializes TextureManager that will hold all textures
    */
    TextureManager();
    /*
    Deletes the TextureManager and all associated textures
    */
    ~TextureManager();

    GLuint LoadEmbeddedTexture(const aiTexture* texture, std::string name);

    GLuint LoadTexture(std::string name);
};