#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "include/window.h"
#include "include/character.h"
#include "stb_image.h"
#include <vector>


int windowWidth = 0;
int windowHeight = 0;

Character* player = nullptr;
GLuint backgroundTex = 0;

//for platforms
GLuint platformTex = 0;
std::vector<Platform> platforms;

GLuint loadTexture(const char* path) {
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

void renderQuad(GLuint texture, float x, float y, float width, float height) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(1, 0); glVertex2f(x + width, y);
    glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
    glTexCoord2f(0, 1); glVertex2f(x, y + height);
    glEnd();
}

int main() {

    int bgWidth, bgHeight, channels;
    unsigned char* bgData = stbi_load("assets/background.png", &bgWidth, &bgHeight, &channels, 4);
    if (!bgData) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }
    stbi_image_free(bgData);
    windowWidth = bgWidth;
    windowHeight = bgHeight;

    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Re-enter the Museum", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    backgroundTex = loadTexture("assets/background.png");

    int charW, charH, ch;
    unsigned char* data = stbi_load("assets/thief1_right.png", &charW, &charH, &ch, 4);
    if (!data) {
        std::cerr << "Failed to load texture to get size!\n";
        return -1;
    }
    stbi_image_free(data);

    float scale = 1.0f / 4.0f;
    float drawW = charW * scale;
    float drawH = charH * scale;

    float posX = windowWidth - drawW;
    float posY = windowHeight - drawH;

    player = new Character(posX, posY, "assets/thief1_right.png", "assets/thief1_left.png");

    platforms.emplace_back(270, 1552, 170, 27, platformTex);//1
    platforms.emplace_back(0, 1350, 220, 27, platformTex);//2
    platforms.emplace_back(0, 1452, 300, 27, platformTex);//3
    platforms.emplace_back(360, 1102, 285, 40, platformTex);//4
    platforms.emplace_back(990, 970, 428, 44, platformTex);//5
    platforms.emplace_back(1550, 1165, 330, 45, platformTex);//6
    platforms.emplace_back(2100, 850, 260, 45, platformTex);//7
    platforms.emplace_back(488, 685, 330, 40, platformTex);//8
    platforms.emplace_back(1148, 505, 400, 43, platformTex);//9
    platforms.emplace_back(1850, 470, 400, 30, platformTex);//10
    


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        renderQuad(backgroundTex, 0, 0, windowWidth, windowHeight);
        player->handleInput(window);
        player->update(1.0f / 60.0f, windowHeight, windowWidth, platforms);
        player->draw();
        for (const auto& p : platforms)
            p.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete player;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}