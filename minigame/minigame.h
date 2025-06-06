#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "include/MiniCharacter.h"
 #include "include/platform.h"
// #include "stb_image.h"
#include <vector>

class MiniGame {
public:
    MiniGame();
    ~MiniGame();

    void init(GLFWwindow* window, std::vector<Platform> platformsToInit);
    void update(GLFWwindow* window);
    void render();
    void cleanup();
    float timeOnLastPlatform = 0.0f;
    bool isFinished() const;
    void updateMinigamePlatforms();

private:
    MiniCharacter* player;
    std::vector<Platform> serverPlatforms; // no scaling, for reference in updating platforms
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