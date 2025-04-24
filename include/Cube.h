#pragma once

#include <vector>

#include "core.h"

class Cube {
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model;
    glm::vec3 color;

    // Cube Information
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

public:
    bool isJumping = false;
    float jumpVelocity = 0.0f;
    float jumpHeight = 0.0f;
    float gravity = -0.00002f;
    float initialJumpVelocity = 0.015f;
    glm::mat4 baseModel = glm::mat4(1.0f);
    bool isfloor = false;
    bool isGrounded = true; 

    Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
    ~Cube();

    void draw(const glm::mat4& viewProjMtx, GLuint shader, bool floor);
    void update();

    void userInput(int key);
    void handleContinuousInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir);
    glm::vec3 getPosition() const;

    void spin(float deg);
};