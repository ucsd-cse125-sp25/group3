#include "TextureManager.h"

TextureManager::TextureManager(){

}

TextureManager::~TextureManager(){
    for (auto it = textures.begin(); it != textures.end(); ++it){
        glDeleteTextures(1, &(it->second));
    }
}

GLuint TextureManager::createTexture(unsigned char* data, int channels, int width, int height, std::string name) {
    GLuint texID;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    //setting texture wrapping to GL-REPEAT for s coord (u coord) and t coord (v coord)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        // std::cout << nrChannels << std::endl;
        if (channels == 3){
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

        textures[name] = texID;
    }
    else
    {
        glDeleteTextures(1, &texID);
        std::cout << "Failed to load texture" << std::endl;
        texID = 0;
    }

    return texID;
}

GLuint TextureManager::LoadEmbeddedTexture(const aiTexture* texture, std::string name){
    unsigned char *mem_data = reinterpret_cast<unsigned char *>(texture->pcData);
    unsigned char *data = nullptr;
    int width = texture->mWidth;
    int height = texture->mHeight;
    int channels = 0;
    bool clear = false;
    if (height == 0) {
        data = stbi_load_from_memory(mem_data, texture->mWidth, &width, &height, &channels, 4);
        clear = true;
    } else {
        data = mem_data;
    }

    GLuint texID =  createTexture(data, channels, width, height, name);

    if (clear) {
        stbi_image_free(data);
    }

    return texID;
}

bool exrToPng(std::string &str) {
    std::string exr = ".exr";
    std::string png = ".png";
    if (str.length() >= exr.length()){
        if (0 == str.compare(str.length() - exr.length(), exr.length(), exr)) {
            str.replace(str.length() - png.length(), png.length(), png);
            std::cout << str << std::endl;
            return true;
        }
        return false;
    } else {
        return false;
    }
}

GLuint TextureManager::LoadTexture(std::string name){

    if (textures.find(name) != textures.end()){
        std::cout << "texture " << name << " already loaded" << std::endl;
        return textures[name];
    }

    std::string texture_file_path = "../textures/"+std::string(name.c_str());

    if (texture_file_path.compare("../textures/Museum Map.fbm/Screenshot 2025-05-18 at 9.01.32 PM.png") == 0){
        texture_file_path = "../textures/Museum Map.fbm/Screenshot 2025-05-18 at.png";
    }

    // if (texture_file_path.compare("../textures/security guard.fbm/body_color.001.exr") == 0){
    //     texture_file_path = "../textures/security guard.fbm/body_color_001.png";
    // }

    exrToPng(texture_file_path);
    
    // load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    unsigned char *data = stbi_load(texture_file_path.c_str(), &width, &height, &nrChannels, 0);

    GLuint texID = createTexture(data, nrChannels, width, height, name);

    stbi_image_free(data);

    return texID;
}