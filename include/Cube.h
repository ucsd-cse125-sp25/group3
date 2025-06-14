#pragma once

#include <vector>
#include <cstdio>
#include "core.h"
#include <iostream>
#include <fstream>
#include "../packets/InitPlayerPacket.h"

#include "Artifact.h"


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

    // for invisible ability
    bool isInvisible = false;
    float invisibleStartTime = 0.0f;
    float invisibleDuration = 5.0f;
    bool eWasPressed = false;

    //to set different color of cube
    void setColor(const glm::vec3& change_color);
    void setBaseModel(const glm::mat4& change_model);
    void setBaseAndModel(const glm::mat4& mtx);
    //interact with artifact
    bool isCarryingArtifact = false;
    void attemptGrabArtifact();
    Artifact* carriedArtifact = nullptr;
    void setCarriedArtifact(Artifact* artifact);

    // for "changing apperance"
    glm::vec3 colorAlt = glm::vec3(0.2f, 0.8f, 1.0f); 
    bool isAltColor = false;  

    enum CharacterType {
        CHARACTER_1,
        CHARACTER_2,
        CHARACTER_3,
        CHARACTER_4,
        
    };
    
    CharacterType type = CHARACTER_1;  // default
    
    Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
    ~Cube();

    void draw(const glm::mat4& viewProjMtx, GLuint shader, bool floor);
    void update();

    void userInput(int key);
    void handleContinuousInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir);
    glm::vec3 getPosition() const;

    void updateFromPacket(const InitPlayerPacket& packet);

    void spin(float deg);

    void printState() {
        std::cout << "base model:" << std::endl;
        std::cout << glm::to_string(baseModel) << std::endl;
    }
    
    // for show-on-map ability 
    bool didUseRadarAbility();
    bool radarUsed = false; 
};