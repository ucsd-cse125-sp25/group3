#pragma once

#include "Cube.h"
#include "AnInstance.h"
#include "ShaderManager.h"
#include "AnimationPlayer.h"
#include "../packets/InitPlayerPacket.h"

class Character {
private:

public:
    bool useModel;
    Cube* cube;                     
    AnInstance* model;                   
    glm::mat4 baseModel = glm::mat4(1.0f);  

    bool isJumping = false;
    float jumpVelocity = 0.0f;
    float jumpHeight = 0.0f;
    float gravity = -0.00005f;
    bool isGrounded = true;
    float initialJumpVelocity = 0.015f;
    bool isInvisible = false;

    glm::vec3 lastMoveDir = glm::vec3(0, 0, 1);  // 初始面向前

    // Character(bool useModel,const std::string& modelPath = "");
    Character(AnInstance* model);  // 新构造函数声明
    ~Character();

    void draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager);
    void update(AnimationPlayer* animationPlayer);

    void triggerJump();
    void handleInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir);
    glm::vec3 getPosition() const;

    void updateFromPacket(const InitPlayerPacket& packet);
};