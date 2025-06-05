#include <GL/glew.h>
#include "character.h"
#include "stb_image.h"
#include "shader_utils.h"
#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>

MinigameCharacter::MinigameCharacter() {}

MinigameCharacter::MinigameCharacter(float x, float y, const std::string& leftTex, const std::string& rightTex)
    : x(x), y(y), vx(0.f), speed(300.f), facingRight(true) {
    textureLeft = loadTexture(leftTex);
    textureRight = loadTexture(rightTex);
    currentTexture = textureLeft;
    initRenderer();
}

unsigned int MinigameCharacter::loadTexture(const std::string& path) {
    int w, h, ch;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    std::cout << "Minigame character width " << w << ", height " << h << std::endl;

    width = w;  
    height = h;
    
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return tex;
}

GLuint MinigameCharacter::getShader() const {
    return characterShader;
}

void MinigameCharacter::initRenderer() {
    float vertices[] = {
        // positions    // tex coords
         0.f,  0.f,      0.0f, 0.0f,
         1.f,  0.f,      1.0f, 0.0f,
         1.f,  1.f,      1.0f, 1.0f,

         0.f,  0.f,      0.0f, 0.0f,
         1.f,  1.f,      1.0f, 1.0f,
         0.f,  1.f,      0.0f, 1.0f
    };

    glGenVertexArrays(1, &characterVAO);
    glGenBuffers(1, &characterVBO);
    glBindVertexArray(characterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, characterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint vs = compileShader("minigame/shaders/quad.vert", GL_VERTEX_SHADER);
    GLuint fs = compileShader("minigame/shaders/quad.frag", GL_FRAGMENT_SHADER);
    characterShader = glCreateProgram();
    glAttachShader(characterShader, vs);
    glAttachShader(characterShader, fs);
    glLinkProgram(characterShader);
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void MinigameCharacter::update(const MinigameCharacterPacket& packet) {
    x = packet.x;
    y = packet.y;
    facingRight = packet.facingRight;
    isFinished = packet.isFinished;
}

void MinigameCharacter::draw() {
    glUseProgram(characterShader);
    glBindVertexArray(characterVAO);

    glActiveTexture(GL_TEXTURE0);
    if (facingRight) currentTexture = textureRight;
    else currentTexture = textureLeft;
    glBindTexture(GL_TEXTURE_2D, currentTexture);
    glUniform1i(glGetUniformLocation(characterShader, "backgroundTex"), 0); 

    float scale = 1.0f / 4.0f;
    float w = width * scale;
    float h = height * scale;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(w, h, 1.0f));

    GLuint modelLoc = glGetUniformLocation(characterShader, "uModel");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

float MinigameCharacter::getX() const {
    return x;
}

float MinigameCharacter::getY() const {
    return y;
}

float MinigameCharacter::getWidth() const {
    return width * 1.0f / 4.0f;
}

float MinigameCharacter::getHeight() const {
    return height * 1.0f / 4.0f;
}
