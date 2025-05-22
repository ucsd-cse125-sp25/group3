#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "include/character.h"
// #include "include/platform.h"
// #include "stb_image.h"
#include <vector>

class MiniGame {
public:
    MiniGame();
    ~MiniGame();

    void init(GLFWwindow* window);
    void update(GLFWwindow* window);
    void render();
    void cleanup();
    bool isFinished() const;

private:
    Character* player;
    std::vector<Platform> platforms;
    GLuint backgroundTex;
    int windowWidth;
    int windowHeight;
    bool finished;

    GLuint quadVAO = 0, quadVBO = 0;
    GLuint quadShader = 0;

    GLuint loadTexture(const char* path);
    void renderQuad(GLuint texture, float x, float y, float width, float height);
    void initQuadRenderer();
};