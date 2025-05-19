#ifndef CHARACTER_H
#define CHARACTER_H

#include <GLFW/glfw3.h>
#include <string>
#include <platform.h>

class Character {
public:
    Character(float x, float y, const std::string& leftTex, const std::string& rightTex);
    void handleInput(GLFWwindow* window);
    void update(float dt, int windowHeight, int windowWidth, const std::vector<Platform>& platforms);
    void draw();

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;


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

    int width, height;
};

#endif