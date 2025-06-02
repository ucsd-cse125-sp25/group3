#pragma once

#include <vector>
#include "core.h"
#include "Cube.h"
#include <chrono>
#include "../packets/NPCPacket.h"
#include "Character.h"


class NPCs {
private:

    glm::vec3 pointA = glm::vec3(-3.0f, 0.0f, 0.0f);
    glm::vec3 pointB = glm::vec3(3.0f, 0.0f, 0.0f);
    bool goingToA = false; 
    float speed = 0.01f;

    bool isWaiting = false;
    std::chrono::steady_clock::time_point waitStartTime;
    float waitDuration = 2.0f; 

public:

    NPCs(Character* character);
    ~NPCs();

    Character* npcModel;

    enum State {
        IDLE,
        WALKING
    };
    State currentState;

    glm::vec3 nextPos;
    glm::vec3 currPos;
    glm::vec3 currentTarget;


    void update();
    void draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager);

    glm::vec3 generatePosition();
    glm::vec3 generateRandomTarget();

    void updateFromPacket(const NPCPacket& npcPacket, AnimationPlayer* animationPlayer);
};