#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
// #include <sys/socket.h>
// #include <unistd.h>
#include <string>
#include "ServerLogic.h"

#include "glm/gtx/string_cast.hpp"

bool ServerLogic::gameStarted = false;
bool ServerLogic::availableChars[4] = {true, true, true, true};
std::string ServerLogic::movingArtifacts[3] = {"horse", "skeleton", "lion"};
std::map<std::string, AABB> ServerLogic::museumAABBs;

CubeState::CubeState(glm::vec3 cubeMin, glm::vec3 cubeMax) {
    // Model matrix.
    model = glm::mat4(1.0f);

    // The color of the cube. Try setting it to something else!
    color = glm::vec3(1.0f, 0.95f, 0.1f);

    // Specify vertex positions
    positions = {
        // Front
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),

        // Back
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),

        // Top
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

        // Bottom
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),

        // Left
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

        // Right
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z)};

    // Specify normals
    normals = {
        // Front
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),

        // Back
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),

        // Top
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),

        // Bottom
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),

        // Left
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),

        // Right
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0)};

    // Specify indices
    indices = {
        0, 1, 2, 0, 2, 3,        // Front
        4, 5, 6, 4, 6, 7,        // Back
        8, 9, 10, 8, 10, 11,     // Top
        12, 13, 14, 12, 14, 15,  // Bottom
        16, 17, 18, 16, 18, 19,  // Left
        20, 21, 22, 20, 22, 23,  // Right
    };
}

void CubeState::printState() {
    std::cout << "base model:" << std::endl;
    std::cout << glm::to_string(baseModel) << std::endl;
}

void CubeState::update() {
    #ifdef __APPLE__
        gravity = -0.00005f;
    #endif
    
    if (isJumping) {
        //update jump height
        jumpVelocity += gravity; 
        jumpHeight += jumpVelocity;

        if (jumpHeight <= 0.0f) {
            jumpHeight = 0.0f;
            isJumping = false;
            jumpVelocity = 0.0f;
            isGrounded = true;
        }
    }

    if (isInvisible && std::chrono::steady_clock::now() - invisibleStartTime > std::chrono::seconds(invisibleDuration)) {
        isInvisible = false;
    }

    if (isSpeedBoosted && std::chrono::steady_clock::now() - speedBoostStartTime > std::chrono::seconds(speedBoostDuration)) {
        isSpeedBoosted = false;
        speed = normalSpeed;
    }

    glm::mat4 rotateM = glm::inverse(glm::lookAt(glm::vec3(0), lastMoveDir, glm::vec3(0, 1, 0)));

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, jumpHeight, 0.0f)) * baseModel;
    model = model * rotateM;

    // printf("jump height: %f\n", jumpHeight);
    //printState();

}

glm::vec3 CubeState::getPosition()const {
    return glm::vec3(model[3]);  // extract translation from matrix
}

void CubeState::updateFromPacket(const InitPlayerPacket& packet) {
    memcpy(&baseModel, packet.model, sizeof(packet.model));
    isInvisible = packet.isInvisible;
    printState();
}

void CubeState::saveToPacket(InitPlayerPacket& packet) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.model[i][j] = model[i][j];
        }
    }
    packet.isInvisible = isInvisible;
    packet.type = type;
    packet.animState = animState;

    // memcpy(buf, &isInvisible, sizeof(bool));
    // vec->insert(vec->end(), &buf[0], &buf[1]);
    // printf("invisible: %hhu\n", vec->back());
}

// PlayerData::PlayerData() {
    
//     cube = new CubeState();
    
//     camera = new Camera();
// }

// PlayerData::~PlayerData() {
//     delete cube;
//     delete camera;
// }

void PlayerData::handleGuiInput(KeyType key) {

    if (currentState == START_MENU) {

        if (key == KeyType::MENU_START) {
            currentState = CHARACTER_SELECTION;
        }
    } else if (currentState == CHARACTER_SELECTION) {

        if (key == KeyType::CHAR_SEL_BACK) {
            currentState = START_MENU;
        }
    }
}

void PlayerData::calculateNewPos(KeyType key, ArtifactState* artifact, 
                const std::map<std::string, AABB> museumAABBs,
                std::map<unsigned int, PlayerData*> playersData,
                std::map<unsigned int, NPCState>& npcData) {

    glm::vec3 forwardDir = camera.GetForwardVector();
    forwardDir.y = 0.0f;  
    forwardDir = glm::normalize(forwardDir);
    glm::vec3 rightDir = glm::normalize(glm::cross(forwardDir, glm::vec3(0, 1, 0)));
    glm::vec3 movement(0.0f);

    if (key == KeyType::KEY_W)
        movement += forwardDir;
    if (key == KeyType::KEY_S)
        movement -= forwardDir;
    if (key == KeyType::KEY_D)
        movement += rightDir;
    if (key == KeyType::KEY_A)
        movement -= rightDir;
        
    // if (glm::length(movement) > 0.0f) {
    //     iterationsStopped = 0;
    //     movement = glm::normalize(movement) * cube.speed;
    //     cube.baseModel = glm::translate(cube.baseModel, movement);
    //     cube.lastMoveDir = glm::normalize(movement);
    //     cube.animState = AnimState::Walk; 
    // }

    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement) * cube.speed;

        glm::vec3 nextPos = glm::vec3(cube.baseModel[3]) + movement;
    
        float characterHalfSize = 0.3f; 
    
        AABB characterBox(
            nextPos - glm::vec3(characterHalfSize),
            nextPos + glm::vec3(characterHalfSize)
        );
    
        bool collided = false;
        for (const auto& [name, box] : museumAABBs) {
            if (box.intersects(characterBox)) {
                collided = true;
                break;
            }
        }

        // other player AABB
        for (const auto& [id, data] : playersData) {
            if (data == this) continue;
            glm::vec3 otherPos = data->cube.getPosition();
            AABB otherBox(
                otherPos - glm::vec3(characterHalfSize),
                otherPos + glm::vec3(characterHalfSize)
            );
            if (characterBox.intersects(otherBox)) {
                std::cout<< "yes collide with other player  " <<std::endl;
                collided = true;
                break;
            }
        }

        // NPC AABB 
        if (!collided) {
        for (const auto& [id, npc] : npcData) {
            glm::vec3 npcPos = npc.npcModel.getPosition();
                AABB npcBox(
                    npcPos - glm::vec3(characterHalfSize),
                    npcPos + glm::vec3(characterHalfSize)
                );
                if (characterBox.intersects(npcBox)) {
                    std::cout<< "yes collide with npc  " <<std::endl;
                    collided = true;
                    break;
                }
            }
        }

    
        if (!collided) {
            cube.baseModel = glm::translate(cube.baseModel, movement);
            cube.lastMoveDir = glm::normalize(movement);
        } else {
            // std::cout << "Collision detected, movement blocked!" << std::endl;
        }
        cube.animState = AnimState::Walk; 
        iterationsStopped = 0;
    }

    if (key == KeyType::KEY_R) {
        resetCamera();
    }

    if (key == KeyType::KEY_ALT_PRESS) {
        altDown = true;
    } else if (key == KeyType::KEY_ALT_RELEASE) {
        altDown = false;
        firstMouse = true;
    }

    // if (key == KeyType::KEY_SPACE) {
    //     // printf("JUMPING\n");
    //     if (cube.isGrounded) {
    //         cube.isJumping = true;
    //         cube.isGrounded = false;   
    //         cube.jumpVelocity = cube.initialJumpVelocity;
    //     }
    // }

    if (key == KeyType::KEY_F) {
        artifact->attemptGrab(&cube);
    }

    // if (key == KeyType::KEY_T)
    //     cube.baseModel = glm::translate(cube.baseModel, glm::vec3(0, 0.005f, 0));

    // if (key == KeyType::KEY_G)
    //     cube.baseModel = glm::translate(cube.baseModel, glm::vec3(0, -0.005f, 0));

    // if (key == KeyType::KEY_K)
    //     cube.baseModel = glm::rotate(cube.baseModel, 0.02f, glm::vec3(0, 1, 0));

    // if (key == KeyType::KEY_L)
    //     cube.baseModel = glm::rotate(cube.baseModel, -0.02f, glm::vec3(0, 1, 0));
    
    if (key == KeyType::KEY_E){
        if (!cube.eWasPressed) {
            switch (cube.type) {
                case CHARACTER_1: {
                    if (!cube.isInvisible) {
                        cube.isInvisible = true;
                        cube.invisibleStartTime = std::chrono::steady_clock::now();
                    }
                    break;
                }
                case CHARACTER_2: {
                    if (!cube.isSpeedBoosted) {
                        cube.isSpeedBoosted = true;
                        cube.speedBoostStartTime = std::chrono::steady_clock::now();
                        cube.speed = cube.boostedSpeed;
                        cube.animState = AnimState::Run;
                    }
                    break;
                }
                case CHARACTER_3: {

                    if (!radarActive) {
                        radarActive = true; 
                        radarStartTime = std::chrono::steady_clock::now(); 
                    }
                    break;
                }
                case CHARACTER_4: {
                    
                    if (!radarActive) {
                        printf("radar activated\n");
                        radarActive = true; 
                        radarStartTime = std::chrono::steady_clock::now(); 
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            cube.eWasPressed = true;
        } else {
            cube.eWasPressed = false;
        }
    }

    if (key == KeyType::KEY_C) {
        ServerLogic::processCapture(this, playersData, npcData);
                std::cout <<"C is pressed " << std::endl;
        if (skillOnCooldown) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - skillCooldownStart);
            if (elapsed.count() < 10) {
                std::cout << "Skill on cooldown!" << std::endl;
                return;
            } else {
                skillOnCooldown = false;
            }
        }
    }
}

bool PlayerData::init(InitPacket* packet, unsigned int client_id) {

    
    if (packet->character == NONE) {
        windowWidth = packet->windowWidth;
        windowHeight = packet->windowHeight;
        lastX = windowWidth / 2.0f;
        lastY = windowHeight / 2.0f;
        currentState = START_MENU;
        cube.client_id = client_id;
        return false;
    } else {

        if (!ServerLogic::availableChars[packet->character]) {
            printf("character already taken\n");
            return false;
        }
        initialized = true;
        firstMouse = true;
        altDown = false;
        radarActive = false;
        miniMapCam.SetOrtho(-10, 10, -10, 10, 0.1f, 100.0f); 
        miniMapCam.SetLookAt(glm::vec3(0, 20, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
        cube.type = packet->character;
        minigame.init();

        if (cube.type == CharacterType::CHARACTER_4) {
            cube.animState = AnimState::SG_Shooting_Gun;
        } else if (cube.type != CharacterType::NONE) {
            cube.animState = AnimState::FT_Idle;
        }
        ServerLogic::availableChars[packet->character] = false;
        currentState = WAITING;
        return true;
    }
    // initialized = false;
    // firstMouse = true;
    // altDown = false;
    // radarActive = false;
    // miniMapCam.SetOrtho(-10, 10, -10, 10, 0.1f, 100.0f); 
    // miniMapCam.SetLookAt(glm::vec3(0, 20, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    // // cube.type = packet->character;
    // // ServerLogic::charSelected[packet->character] = true;
    
    // return true;
    // printf("character: %d, width: %d, height: %d\n", cube.type, windowWidth, windowHeight);
}

void PlayerData::handleCursor(double currX, double currY) {
// printf("client id: %u\n", client_id);
    if (altDown) {
        return;
    }
    
    float sensitivity = 0.1f;

    if (firstMouse) {
        lastX = currX;
        lastY = currY;
        firstMouse = false;
    }

    float xoffset = currX - lastX;
    float yoffset = currY - lastY; 

    lastX = currX;
    lastY = currY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    
    float newAzimuth = camera.GetAzimuth() + xoffset;
    float newIncline = glm::clamp(camera.GetIncline() + yoffset, -89.0f, 89.0f); 

    camera.SetAzimuth(newAzimuth);
    camera.SetIncline(newIncline);
    
    // cube.update();
    // camera.Update(cube.getPosition()); 
}

void PlayerData::detectStop() {
    iterationsStopped += 1;
// printf("here: %d\n", iterationsStopped);
    if (iterationsStopped == NUM_TO_STOP && cube.type == CharacterType::CHARACTER_4) {
        cube.animState = AnimState::SG_Shooting_Gun;
        iterationsStopped = 0;
    } else if (iterationsStopped == NUM_TO_STOP && cube.type != CharacterType::NONE) {
        iterationsStopped = 0;
        cube.animState = AnimState::FT_Idle;
    }
}

void PlayerData::resetCamera() {
    camera.Reset();
    camera.SetAspect(float(windowWidth) / float(windowHeight));
}

void PlayerData::saveToPacket(unsigned int client_id, InitPlayerPacket& packet) {
    packet.clientID = client_id;

    cube.saveToPacket(packet);
    camera.saveToPacket(packet, false);
    miniMapCam.saveToPacket(packet, true); 
    packet.altDown = altDown;
}

void PlayerData::updateRadarAbility() {

    if (radarActive) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - radarStartTime);

        if (elapsed.count() > radarDuration) {
            radarActive = false;
        }
    }
}

void PlayerData::update() {

    if (!initialized) return;

    updateRadarAbility();
    cube.update();
    camera.Update(cube.getPosition()); 
}

NPCState::NPCState(){
    npcModel = CubeState(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));
    npcModel.animState = AnimState::Walk;
    glm::vec3 startPos = glm::vec3(2.0f, 0.0f, -3.0f);
    glm::mat4 newModel = glm::mat4(1.0f);
    newModel = glm::translate(newModel, startPos);

    npcModel.model = newModel;
    currentTarget = generateRandomTarget();
}

void NPCState::update(const std::map<std::string, AABB> museumAABBs){
    
    // float random_num = static_cast <float> (rand())/ (static_cast <float> (RAND_MAX ));

    // // std::cout<< random_num << std::endl;
    // if (random_num < 0.5 ){
    //     // std::cout<< "stop" << std::endl;

    //     return;
    // }

    using namespace std::chrono;

    if (isWaiting) {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<duration<float>>(now - waitStartTime).count();

        if (elapsed >= waitDuration) {
            isWaiting = false;
            npcModel.animState = AnimState::Walk;
            currentTarget = generateRandomTarget();  
        } else {
            return;  
        }
    }
    glm::vec3 currPos = npcModel.getPosition();

    glm::vec3 direction = currentTarget - currPos;

    if (glm::length(direction) < 0.05f || collided) {
        float random_num = static_cast <float> (rand())/ (static_cast <float> (RAND_MAX));

        if (random_num < 0.45 ){
            isWaiting = true;
            npcModel.animState = AnimState::FT_Idle;
        }
        waitStartTime = std::chrono::steady_clock::now();
        currentTarget = generateRandomTarget(); 
        collided = false;
        return;
    }
    // glm::vec3 movement = glm::normalize(direction) * speed;
    // npcModel.baseModel = glm::translate(npcModel.baseModel, movement);
    // npcModel.lastMoveDir = glm::normalize(movement);
    // glm::mat4 rotateM = glm::inverse(glm::lookAt(glm::vec3(0), npcModel.lastMoveDir, glm::vec3(0, 1, 0)));
    // npcModel.model = npcModel.baseModel * rotateM;
    
    glm::vec3 movement = glm::normalize(direction) * speed;
    glm::vec3 nextPos = glm::vec3(npcModel.baseModel[3]) + movement;
    float characterHalfSize = 0.3f; 

    AABB characterBox(
        nextPos - glm::vec3(characterHalfSize),
        nextPos + glm::vec3(characterHalfSize)
    );
    // collided = false;

    for (const auto& [name, box] : museumAABBs) {
        if (box.intersects(characterBox)) {
            collided = true;
            break;
        }
    }

    if (!collided) {
        npcModel.baseModel = glm::translate(npcModel.baseModel, movement);
        npcModel.lastMoveDir = glm::normalize(movement);
        glm::mat4 rotateM = glm::inverse(glm::lookAt(glm::vec3(0), npcModel.lastMoveDir, glm::vec3(0, 1, 0)));
        npcModel.model = npcModel.baseModel * rotateM;
    } 
    // npcModel.model = npcModel.baseModel;
    // npcModel->setBaseAndModel(glm::translate(npcModel->baseModel, movement));
}

glm::vec3 NPCState::generateRandomTarget() {
    float x = -8.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (16.0f))); // [-8, 8]
    float z = -8.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (16.0f))); // [-8, 8]
    // std::cout<< x << "   " << z << std::endl;
    return glm::vec3(x, 0.0f, z);
}

void NPCState::saveToPacket(NPCPacket& packet) {
    packet.animState = npcModel.animState;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.model[i][j] = npcModel.model[i][j];
        }
    }
}

ArtifactState::ArtifactState() {
    init_state = 0;
    holder = nullptr;
    glm::vec3 min = glm::vec3(-0.5, 0, -1);
    glm::vec3 max = glm::vec3(0, 0.5, 1);
    calcRadius(min, max);
    artifactModel = CubeState(min, max);
    artifactModel.model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.0f, 2.0f));
}

void ArtifactState::init(glm::vec3 minCube, glm::vec3 maxCube, glm::vec3 position, unsigned int artifact_id) {
    init_state = 0;
    calcRadius(minCube, maxCube);
    artifactModel = CubeState(minCube, maxCube);
    artifactModel.model = glm::translate(glm::mat4(1.0f), position);
    id = artifact_id;
}

void ArtifactState::resetPos() {
    init_state = 0;
    glm::vec3 position = ServerLogic::museumAABBs[ServerLogic::movingArtifacts[id]].getCenter();
    artifactModel.model = glm::translate(glm::mat4(1.0f), position);
    // id = artifact_id;
}

void ArtifactState::calcRadius(glm::vec3 min, glm::vec3 max){
    radius = glm::distance(min, max)/2.0f;
}

void ArtifactState::update(bool putDown) {

    if (holder != nullptr && !putDown) {
        glm::vec3 offset(1.0f, 1.0f, 0.0f);
        glm::mat4 newBaseModel = glm::translate(holder->baseModel, offset);
        artifactModel.model = newBaseModel;
    } else if (holder != nullptr) {
        init_state = 2;
        glm::vec3 offset(1.0f, 0.0f, 0.0f);
        glm::mat4 newBaseModel = glm::translate(holder->baseModel, offset);
        artifactModel.model = newBaseModel;
    }
}

void ArtifactState::attemptGrab(CubeState * player) {
    //for now, don't allow someone to grab artifact if already held by someone else
    // printf("attempting grab by player %d\n", player->type);
    if (holder == nullptr) {
        glm::vec3 playerPos = player->getPosition();
        glm::vec3 artifactPos = artifactModel.getPosition();
        float distance = glm::length(playerPos - artifactPos);
        // std::cout << "Distance = " << distance << std::endl;
        // printf("distance: %f\n", distance);
        std::cout << "dist: " << distance << std::endl;
        std::cout << "radius: " << radius << std::endl;
        if (distance < radius + 2.0f) {

            if (player->type == CharacterType::CHARACTER_4) {
                resetPos();
                return;
            }
            holder = player;
            init_state = 1;
            // player->animState = AnimState::FT_Pick_Up;
            player->isCarrying = true;
            printf("artifact picked up\n");
        }
    } else if (holder == player) {
        update(true);
        holder = nullptr;
        player->isCarrying = false;
    }
}

void ArtifactState::saveToPacket(StateUpdatePacket& packet) {
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.artifactModel[i][j] = artifactModel.model[i][j];
        }
    }

    packet.artifact_state = init_state;
}

bool ServerLogic::processMovement(std::set<KeyType>& recievedMovementKeys, KeyType key) {

    if (recievedMovementKeys.find(key) == recievedMovementKeys.end()) {
        recievedMovementKeys.insert(key);
        return true;
    }
    return false;
}

void ServerLogic::attemptGameStart(std::map<unsigned int, PlayerData*>& playersData) {
    std::map<unsigned int, PlayerData*>::iterator playerIter;
    int numPlayers = 0;

    if (!gameStarted) {
        // printf("trying to start game\n");
        for (playerIter=playersData.begin(); playerIter!=playersData.end(); playerIter++) {

            if (playerIter->second->currentState == WAITING) {
                numPlayers++;
            }
        }

        if (numPlayers < TOTAL_PLAYERS) {
            return;
        }
    }
    // printf("starting\n");
    for (playerIter=playersData.begin(); playerIter!=playersData.end(); playerIter++) {
        
        if (playerIter->second->currentState == WAITING) {
            playerIter->second->currentState = PLAYING;
        }
    }

    gameStarted = true;
}

void ServerLogic::loadAABBs() {
    // load collision box 
    // museumAABBs = AABB_loader::loadAABBs("../models/map_bb/museum_wall_aabb.obj");
    // std::map<std::string, AABB> artifactsAABBs = AABB_loader::loadAABBs("../models/map_bb/artefacts_aabb.obj");
    // std::map<std::string, AABB> otherAABBs = AABB_loader::loadAABBs("../models/map_bb/non_artefacts_aabb.obj");
    // museumAABBs.insert(artifactsAABBs.begin(), artifactsAABBs.end());
    // // museumAABBs.insert(otherAABBs.begin(), otherAABBs.end());
    
    museumAABBs = AABB_loader::loadAABBs("../models/map_bb/museum_nofloor_bb_large.obj");
    std::map<std::string, AABB> artifactsAABBs = AABB_loader::loadAABBs("../models/map_bb/artefacts_bb_large.obj");
    // std::map<std::string, AABB> otherAABBs = AABB_loader::loadAABBs("../models/map_bb/museum_no_artefact_bb_large.obj");
    museumAABBs.insert(artifactsAABBs.begin(), artifactsAABBs.end());
    // museumAABBs.insert(otherAABBs.begin(), otherAABBs.end());

    // debug print
    for (const auto& [name, box] : museumAABBs) {
        std::cout << "Loaded AABB: " << name 
                  << " Min: " << glm::to_string(box.min) 
                  << " Max: " << glm::to_string(box.max) << std::endl;
    }
}

bool ServerLogic::winCondition(CubeState * player)
{

    if(player->type!=CharacterType::CHARACTER_4 && player->type != CharacterType::NONE)
    {
        
        if(player->isCarrying)
        {
            //std::cout << "enter player carrying check" << std::endl;
            if(player->getPosition().x < -12.8f)
            {
                //std::cout << "enter palyer position check" << std::endl;
                //std::cout << player->getPosition().x << std::endl;
                //currentState = WIN_CONDITION;
                return true;
            }
        }

    }
    return false;

}

void ServerLogic::processCapture(PlayerData* capturer, std::map<unsigned int, PlayerData*>& playersData,std::map<unsigned int, NPCState>& npcData) {
    // std::cout << "enter process Capture function " << std::endl;
    if (capturer->cube.type != CHARACTER_4) return;
    std::cout << "yes character_4 " << std::endl;
    glm::vec3 capturerPos = capturer->cube.getPosition();
    float captureRange = 1.5f;
    bool capturedSomeone = false;

    for (auto& [id, player] : playersData) {
        if (player == capturer) continue;
        if (capturedSomeone) break;

        CharacterType otherType = player->cube.type;

        if (otherType == CHARACTER_1 || otherType == CHARACTER_2 || otherType == CHARACTER_3) {
            std::cout << "yes inside for loop: " << otherType << std::endl;
            glm::vec3 otherPos = player->cube.getPosition();
            float distance = glm::length(capturerPos - otherPos);
            std::cout << "distance: " << distance;
            std::cout << " captureRange : " << captureRange << std::endl;
            if (player->cube.isCaptured){
                std::cout <<  "isCapture is true" << std::endl;
            }else{
                std::cout <<  "isCapture is false"  << std::endl;
            }
            if (distance < captureRange && !player->cube.isCaptured) {
                player->cube.isCaptured = true;
                player->currentState = IN_MINIGAME;
                player->cube.isInvisible = true;
                capturedSomeone = true;
                std::cout << "Player " << id << " captured by hunter!" << std::endl;
                
                capturer->captureMessage = "Thief captured!";
                capturer->captureMessageStart = std::chrono::steady_clock::now();
                capturer->showCaptureMessage = true;

                // TODO : 
                // enter mini game 
                // is Capture = true 
                // after success in mini game, is capture = false 
                break;
            }
        }

    }

    for (auto& [id, npc] : npcData) {

        glm::vec3 npcPos = npc.npcModel.getPosition();
        float distance = glm::length(capturerPos - npcPos);
        if (distance < captureRange) {
            std::cout << "NPC " << id << " captured by hunter!" << std::endl;

            capturer->captureMessage = "Sorry, wrong NPC!";
            capturer->captureMessageStart = std::chrono::steady_clock::now();
            capturer->showCaptureMessage = true;

            capturer->skillOnCooldown = true;
            capturer->skillCooldownStart = std::chrono::steady_clock::now();
            break;
        }

    }
}

std::string ServerLogic::getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
    return ss.str();
}

// void ServerLogic::sendTimeToClient(int clientSocket) {
//     std::string timeStr = getCurrentTimeString();
//     send(clientSocket, timeStr.c_str(), timeStr.length(), 0);
// }

void MinigameState::init() {
    float refW = 2880.0f;
    float refH = 1800.0f;

    platforms.emplace_back((330.0f / refW), (1700.0f / refH), (200.0f / refW), (27.0f / refH), 0);
    platforms.emplace_back((0.0f   / refW), (1475.0f / refH), (270.0f / refW), (27.0f / refH), 0);
    platforms.emplace_back((0.0f   / refW), (1595.0f / refH), (360.0f / refW), (32.0f / refH), 0);
    platforms.emplace_back((468.0f / refW), (1212.0f / refH), (360.0f / refW), (43.0f / refH), 0);
    platforms.emplace_back((1205.0f / refW), (1060.0f / refH), (520.0f / refW), (62.0f / refH), 0);
    platforms.emplace_back((1890.0f / refW), (1268.0f / refH), (408.0f / refW), (58.0f / refH), 0);
    platforms.emplace_back((2570.0f / refW), (930.0f / refH), (320.0f / refW), (55.0f / refH), 0);
    platforms.emplace_back((520.0f / refW), (750.0f / refH), (410.0f / refW), (54.0f / refH), 0);
    platforms.emplace_back((1400.0f / refW), (555.0f / refH), (480.0f / refW), (55.0f / refH), 0);
    platforms.emplace_back((2270.0f / refW), (510.0f / refH), (480.0f / refW), (37.0f / refH), 0);
}
