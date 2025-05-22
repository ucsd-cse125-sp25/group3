#include <GL/glew.h>
#include "character.h"
#include "stb_image.h"
#include "shader_utils.h"
#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>


Character::Character(float x, float y, const std::string& leftTex, const std::string& rightTex)
    : x(x), y(y), vx(0.f), speed(300.f), facingRight(true) {
    textureLeft = loadTexture(leftTex);
    textureRight = loadTexture(rightTex);
    currentTexture = textureLeft;
    initRenderer();
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

GLuint Character::getShader() const {
    return characterShader;
}

void Character::initRenderer() {
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

        bool bottomContact = (y >= p.y + p.height) && (nextY < p.y + p.height);
        if (isJumping && horizontalOverlap && bottomContact) {
            nextY = p.y + p.height;
            vy = 0.f;
        }
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
    glUseProgram(characterShader);
    glBindVertexArray(characterVAO);

    glActiveTexture(GL_TEXTURE0);
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

float Character::getX() const {
    return x;
}

float Character::getY() const {
    return y;
}

float Character::getWidth() const {
    return width * 1.0f / 4.0f;
}

float Character::getHeight() const {
    return height * 1.0f / 4.0f;
}
