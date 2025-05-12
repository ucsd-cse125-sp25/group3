#include <GL/glew.h>
#include "character.h"
#include "stb_image.h"
#include <iostream>

Character::Character(float x, float y, const std::string& leftTex, const std::string& rightTex)
    : x(x), y(y), vx(0.f), speed(300.f), facingRight(true) {
    textureLeft = loadTexture(leftTex);
    textureRight = loadTexture(rightTex);
    currentTexture = textureLeft;
}

unsigned int Character::loadTexture(const std::string& path) {
    int w, h, ch;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;
    }

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

void Character::handleInput(GLFWwindow* window) {
    vx = 0.f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vx = -speed;
        if (facingRight) {
            currentTexture = textureRight;
            facingRight = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vx = speed;
        if (!facingRight) {
            currentTexture = textureLeft;
            facingRight = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isOnGround) {
    vy = jumpVelocity;
    isOnGround = false;
}
}

void Character::update(float dt, int windowHeight, int windowWidth, const std::vector<Platform>& platforms) {
    float drawW = width * 1.0f/4.0f;
    float drawH = height * 1.0f/4.0f;

    float nextX = x + vx * dt;
    float nextY = y + vy * dt;

    isOnGround = false;
    vy += gravity * dt; 

    float groundY =  windowHeight - drawH;
    if (y > groundY) {
        y = groundY;
        vy = 0.f;
        isOnGround = true;
    }

    for (const auto& p : platforms) {
        
        bool isFalling = vy >= 0.f;
        bool isJumping = vy < 0.f;

        bool horizontalOverlap = (nextX + drawW > p.x) && (nextX < p.x + p.width);
        bool verticalContact = (y + drawH <= p.y) && (nextY + drawH >= p.y);

        if (isFalling && horizontalOverlap && verticalContact) {
            nextY = p.y - drawH;
            vy = 0.f;
            isOnGround = true;
        }

        bool verticalOverlap = (nextY + drawH > p.y) && (nextY < p.y + p.height);

        bool hittingLeft = (nextX + drawW > p.x) && (x + drawW <= p.x);
        bool hittingRight = (nextX < p.x + p.width) && (x >= p.x + p.width);

        if (verticalOverlap && hittingLeft) {
            nextX = p.x - drawW;
            vx = 0.f;
        } else if (verticalOverlap && hittingRight) {
            nextX = p.x + p.width;
            vx = 0.f;
        }

        // bool bottomContact = (y >= p.y + p.height) && (nextY < p.y + p.height);
        // if (isJumping && horizontalOverlap && bottomContact) {
        //     nextY = p.y + p.height;
        //     vy = 0.f;
        // }
    }
    x = nextX;
    y = nextY;

    if (x < 0.f) {
        x = 0.f;
        vx = 0.f;
    }
    if (x + drawW > windowWidth) {
        x = windowWidth - drawW;
        vx = 0.f;
    }
}

void Character::draw() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, currentTexture);

    float w = width/4, h = height/4;
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(1, 0); glVertex2f(x + w, y);
    glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
    glTexCoord2f(0, 1); glVertex2f(x, y + h);
    glEnd();
}
