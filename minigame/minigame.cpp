// minigame/MiniGame.cpp
#include "minigame.h"
#include "include/stb_image.h"
#include <iostream>

MiniGame::MiniGame() 
    : player(nullptr), backgroundTex(0), windowWidth(0), windowHeight(0), finished(false) {}

MiniGame::~MiniGame() {
    cleanup();
}

void MiniGame::init(GLFWwindow* window) {
    // Get framebuffer size (in case window was resized)
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    backgroundTex = loadTexture("minigame/assets/background.png");
    if (backgroundTex == 0) {
        finished = true;
        return;
    }

    int charW, charH, ch;
    unsigned char* data = stbi_load("minigame/assets/thief1_right.png", &charW, &charH, &ch, 4);
    if (!data) {
        std::cerr << "Failed to load character texture!\n";
        finished = true;
        return;
    }
    stbi_image_free(data);

    float scale = 1.0f / 4.0f;
    float drawW = charW * scale;
    float drawH = charH * scale;
    float posX = windowWidth - drawW;
    float posY = windowHeight - drawH;

    player = new Character(posX, posY, 
        "minigame/assets/thief1_right.png", 
        "minigame/assets/thief1_left.png");

    // Add platforms (same as your original main)
    platforms.emplace_back(270, 1552, 170, 27, 0);//1
    platforms.emplace_back(0, 1350, 220, 27, 0);//2
    platforms.emplace_back(0, 1452, 300, 27, 0);//3
    platforms.emplace_back(380, 1102, 285, 40, 0);//4
    platforms.emplace_back(990, 970, 428, 44, 0);//5
    platforms.emplace_back(1550, 1165, 330, 45, 0);//6
    platforms.emplace_back(2100, 850, 260, 45, 0);//7
    platforms.emplace_back(420, 680, 330, 40, 0);//8
    platforms.emplace_back(1148, 505, 400, 43, 0);//9
    platforms.emplace_back(1850, 470, 400, 30, 0);//10
}

void MiniGame::update(GLFWwindow* window) {
    if (!player) return;

    player->handleInput(window);
    player->update(1.0f / 60.0f, windowHeight, windowWidth, platforms);


    Platform& lastPlatform = platforms.back();

    float px = player->getX();
    float py = player->getY();
    float pw = player->getWidth();
    float ph = player->getHeight();

    float lx = lastPlatform.getX();
    float ly = lastPlatform.getY();
    float lw = lastPlatform.getWidth();

    bool onPlatform = 
    py + ph >= ly && py + ph <= ly + 15 &&  
    px + pw >= lx && px <= lx + lw;

    if (onPlatform) {
        finished = true;
    }
}

void MiniGame::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderQuad(backgroundTex, 0, 0, windowWidth, windowHeight);

    if (player) player->draw();
    for (const auto& p : platforms)
        p.draw();
}

void MiniGame::cleanup() {
    delete player;
    player = nullptr;
    platforms.clear();
    if (backgroundTex) {
        glDeleteTextures(1, &backgroundTex);
        backgroundTex = 0;
    }
}

bool MiniGame::isFinished() const {
    return finished;
}

GLuint MiniGame::loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}

void MiniGame::renderQuad(GLuint texture, float x, float y, float width, float height) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(1, 0); glVertex2f(x + width, y);
    glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
    glTexCoord2f(0, 1); glVertex2f(x, y + height);
    glEnd();
}
