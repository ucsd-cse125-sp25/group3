#ifndef MINIGAMECHARACTER_H
#define MINIGAMECHARACTER_H

#include <GLFW/glfw3.h>
#include <string>
#include "platform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../packets/MinigameCharacterPacket.h"

class MinigameCharacter {
public:
    MinigameCharacter();
    MinigameCharacter(float x, float y, const std::string& leftTex, const std::string& rightTex);
    void update(const MinigameCharacterPacket& packet);
    void draw();

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    GLuint getShader() const;
    bool isFinished;


private:
    float x, y;
    float vx;
    float vy;
    float speed;
    bool facingRight;

    float gravity = 510.f; 
    float jumpVelocity = -550.f;
    bool isOnGround = true; 

    unsigned int textureLeft;
    unsigned int textureRight;
    unsigned int currentTexture;

    unsigned int loadTexture(const std::string& path);

    GLuint characterVAO = 0;
    GLuint characterVBO = 0;
    GLuint characterShader = 0;
    void initRenderer();

    int width, height;
};

#endif