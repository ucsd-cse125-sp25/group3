#pragma once

#include <vector>

#include "core.h"

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
};

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

    BoundingBox bbox;

public:

    bool isLocalPlayer = true;

    bool isJumping = false;
    float jumpVelocity = 0.0f;
    float jumpHeight = 0.0f;
    float gravity = -0.00002f;
    float initialJumpVelocity = 0.015f;
    glm::mat4 baseModel = glm::mat4(1.0f);
    bool isfloor = false;
    bool isGrounded = true; 

    // for accelaration ability
    float normalSpeed = 0.02f;
    float boostedSpeed = 0.06f;
    float speed = normalSpeed;

    bool isSpeedBoosted = false;
    float speedBoostStartTime = 0.0f;
    float speedBoostDuration = 5.0f;

    // for invisibel ability
    bool isInvisible = false;
    float invisibleStartTime = 0.0f;
    float invisibleDuration = 5.0f;
    bool eWasPressed = false;

    // for "changing apperance"
    glm::vec3 colorAlt = glm::vec3(0.2f, 0.8f, 1.0f); 
    bool isAltColor = false;  

    enum CharacterType {
        CHARACTER_1,
        CHARACTER_2,
        CHARACTER_3,
        CHARACTER_4
    };
    
    CharacterType type = CHARACTER_1;  // default
    



    Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
    ~Cube();

    void draw(const glm::mat4& viewProjMtx, GLuint shader, bool floor);
    void update();

    void userInput(int key);
    void handleContinuousInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir);
    glm::vec3 getPosition() const;

    BoundingBox Cube::calcBBox(const std::vector<glm::vec3>& verts);
    BoundingBox getWorldBBox();
    bool isColliding(Cube* entity);
    std::vector<Cube*> checkCollision(std::vector<Cube*>& scene);

    void spin(float deg);
};