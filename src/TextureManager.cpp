#include "TextureManager.h"

TextureManager::TextureManager(){

}

TextureManager::~TextureManager(){
    for (auto it = textures.begin(); it != textures.end(); ++it){
        glDeleteTextures(1, &(it->second));
    }
}

GLuint TextureManager::LoadTexture(std::string texture_file_path){

    if (textures.find(texture_file_path) != textures.end()){
        std::cout << "texture " << texture_file_path << " already loaded" << std::endl;
        return textures[texture_file_path];
    }

    GLuint texID;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    //setting texture wrapping to GL-REPEAT for s coord (u coord) and t coord (v coord)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    unsigned char *data = stbi_load(texture_file_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        std::cout << nrChannels << std::endl;
        if (nrChannels == 3){
            std::cout << "no transparency" << std::endl;
            // set texture to be image data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << "transparency" << std::endl;
            // set texture to be image data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        // generate a mip map so small meshes don't look weird and we don't waste compute/memory
        glGenerateMipmap(GL_TEXTURE_2D);

        textures[texture_file_path] = texID;
    }
    else
    {
        glDeleteTextures(1, &texID);
        std::cout << "Failed to load texture" << std::endl;
        texID = 0;
    }

    stbi_image_free(data);

    return texID;
}