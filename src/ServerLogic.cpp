#include "ServerLogic.h"

bool ServerLogic::gameStarted = false;
bool ServerLogic::availableChars[4] = {true, true, true, true};

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

    model = glm::translate(baseModel, glm::vec3(0.0f, jumpHeight, 0.0f));
    // printf("jump height: %f\n", jumpHeight);
    //printState();
}

glm::vec3 CubeState::getPosition() {
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

void PlayerData::calculateNewPos(KeyType key, ArtifactState* artifact) {
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
        
    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement) * cube.speed;
        cube.baseModel = glm::translate(cube.baseModel, movement);
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

    if (key == KeyType::KEY_SPACE) {
        // printf("JUMPING\n");
        if (cube.isGrounded) {
            cube.isJumping = true;
            cube.isGrounded = false;   
            cube.jumpVelocity = cube.initialJumpVelocity;
        }
    }

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
                    }
                    break;
                }
                case CHARACTER_3: {
                    // isAltColor = !isAltColor;
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
}

bool PlayerData::init(InitPacket* packet) {
    if (packet->character == NONE) {
        windowWidth = packet->windowWidth;
        windowHeight = packet->windowHeight;
        currentState = START_MENU;
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
        ServerLogic::availableChars[packet->character] = false;
        minigameCharacter = MinigameCharacterState(0, 0);
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

    if (altDown) {
        return;
    }
    static float lastX = windowWidth / 2.0f;
    static float lastY = windowHeight / 2.0f;
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
    glm::vec3 startPos = glm::vec3(2.0f, 0.0f, -3.0f);
    glm::mat4 newModel = glm::mat4(1.0f);
    newModel = glm::translate(newModel, startPos);

    npcModel.model = newModel;
    currentTarget = generateRandomTarget();
}

void NPCState::update(){
    
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
            currentTarget = generateRandomTarget();  
        } else {
            return;  
        }
    }
    glm::vec3 currPos = npcModel.getPosition();

    glm::vec3 direction = currentTarget - currPos;

    if (glm::length(direction) < 0.05f) {
        float random_num = static_cast <float> (rand())/ (static_cast <float> (RAND_MAX ));

        if (random_num < 0.5 ){
            isWaiting = true;
        }
        waitStartTime = std::chrono::steady_clock::now();
        currentTarget = generateRandomTarget(); 
        return;
    }
    glm::vec3 movement = glm::normalize(direction) * speed;
    npcModel.baseModel = glm::translate(npcModel.baseModel, movement);
    npcModel.model = npcModel.baseModel;
    // npcModel->setBaseAndModel(glm::translate(npcModel->baseModel, movement));
}

glm::vec3 NPCState::generateRandomTarget() {
    float x = -8.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (16.0f))); // [-8, 8]
    float z = -8.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (16.0f))); // [-8, 8]
    // std::cout<< x << "   " << z << std::endl;
    return glm::vec3(x, 0.0f, z);
}

void NPCState::saveToPacket(NPCPacket& packet) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.model[i][j] = npcModel.model[i][j];
        }
    }
}

ArtifactState::ArtifactState() {
    holder = nullptr;
    artifactModel = CubeState(glm::vec3(-0.5, 0, -1), glm::vec3(0, 0.5, 1));
    artifactModel.model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.0f, 2.0f));
}

void ArtifactState::update() {

    if (holder != nullptr) {
        glm::vec3 offset(1.0f, 1.0f, 0.0f);
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
        if (distance < 1.5f) {
            holder = player;
            printf("artifact picked up\n");
        }
    }
}

void ArtifactState::saveToPacket(StateUpdatePacket& packet) {
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.artifactModel[i][j] = artifactModel.model[i][j];
        }
    }
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
        printf("trying to start game\n");
        for (playerIter=playersData.begin(); playerIter!=playersData.end(); playerIter++) {

            if (playerIter->second->currentState != WAITING) {
                return;
            }
            numPlayers++;
        }

        if (numPlayers != TOTAL_PLAYERS) {
            return;
        }
    }
    printf("starting\n");
    for (playerIter=playersData.begin(); playerIter!=playersData.end(); playerIter++) {
        
        if (playerIter->second->currentState == WAITING) {
            //playerIter->second->currentState = PLAYING;
            playerIter->second->currentState = IN_MINIGAME;
        }
    }
    gameStarted = true;
}

MinigameCharacterState::MinigameCharacterState()
    : x(0), y(0), vx(0.f), speed(300.f), facingRight(true) {}

MinigameCharacterState::MinigameCharacterState(float x, float y)
    : x(x), y(y), vx(0.f), speed(300.f), facingRight(true) {}

void MinigameCharacterState::handleInput(KeyType key) {
    std::cout << "MinigameCharacterState :: Handling input" << std::endl;
    if (key == KeyType::KEY_A) {
        vx = -speed;
        facingRight = false;
    }
    else if (key == KeyType::KEY_D) {
        vx = speed;
        facingRight = true;
    }
    else if (key == KeyType::KEY_SPACE) {
        vy = jumpVelocity;
        isOnGround = false;
    }
    update();
}

void MinigameCharacterState::update() {
    std::cout << "MinigameCharacterState :: Handling update" << std::endl;
    const float dt = 1.0f / 60.0f;  // fixed timestep for server

    vy += gravity * dt;

    float nextX = x + vx * dt;
    float nextY = y + vy * dt;

    minigame->updateCharacterPosition(this, nextX, nextY);

    // Reset horizontal movement unless continuous input is held
    vx = 0.0f;
}

MiniGameState::MiniGameState()
    : windowWidth(0), windowHeight(0), platforms() {};

MiniGameState::MiniGameState(int windowWidth, int windowHeight, std::vector<Platform> platforms)
    : windowWidth(windowWidth), windowHeight(windowHeight), platforms(platforms) {}

void MiniGameState::updateCharacterPosition(MinigameCharacterState* character, float nextX, float nextY) {
    std::cout << "MiniGameState :: Calculating character position" << std::endl;
    const float drawW = character->width;
    const float drawH = character->height;

    float finalX = nextX;
    float finalY = nextY;

    //character->isOnGround = false;

    std::cout << "MiniGameState :: About to calculate platforms " << std::endl;
    for (int i = 0 ; i < platforms.size(); i++) {
        std::cout << "MiniGameState :: calculating platform " << i << std::endl;
        const float margin = 20.0f;
        bool isFalling = character->vy >= 0.f;
        bool isJumping = character->vy < 0.f;

        // Horizontal and vertical overlap checks
        bool horizontalOverlap = (finalX + drawW > platforms[i].x) && (finalX < platforms[i].x + platforms[i].width);
        bool verticalContact = (character->y + drawH <= platforms[i].y + margin) && (finalY + drawH >= platforms[i].y);

        // Landing on top of platform
        if (isFalling && horizontalOverlap && verticalContact) {
            finalY = platforms[i].y - drawH;
            character->vy = 0.f;
            character->isOnGround = true;
        }

        bool verticalOverlap = (finalY + drawH > platforms[i].y) && (finalY < platforms[i].y + platforms[i].height);
        bool hittingLeft = (finalX + drawW > platforms[i].x) && (character->x + drawW <= platforms[i].x);
        bool hittingRight = (finalX < platforms[i].x + platforms[i].width) && (character->x >= platforms[i].x + platforms[i].width);

        // Left/Right wall collision
        if (verticalOverlap && hittingLeft) {
            finalX = platforms[i].x - drawW;
            character->vx = 0.f;
        } else if (verticalOverlap && hittingRight) {
            finalX = platforms[i].x + platforms[i].width;
            character->vx = 0.f;
        }

        // Hitting bottom of a platform
        bool bottomContact = (character->y >= platforms[i].y + platforms[i].height) && (finalY < platforms[i].y + platforms[i].height);
        if (isJumping && horizontalOverlap && bottomContact) {
            finalY = platforms[i].y + platforms[i].height;
            character->vy = 0.f;
        }
    }

    character->x = finalX;
    character->y = finalY;

    // make sure character doesn't go out of bounds
    if (character->x < 0.0f) {
        character->x = 0.0f;
        character->vx = 0.0f;
    }
    if (character->x + drawW > windowWidth) {
        character->x = windowWidth - drawW;
        character->vx = 0.0f;
    }

    if (character->y + drawH > windowHeight) {
        character->y = windowHeight - drawH;
        character->vy = 0.0f;
        character->isOnGround = true;
    }

    std::cout << "MiniGameState :: new x position " << character->x << ", new y position " << character->y << std::endl;

    // check if player has finished game
    Platform& lastPlatform = platforms.back();

    float px = character->x;
    float py = character->y;
    float pw = character->width;
    float ph = character->height;

    float lx = lastPlatform.x;
    float ly = lastPlatform.y;
    float lw = lastPlatform.width;

    bool onPlatform = 
    py + ph >= ly && py + ph <= ly + 15 &&  
    px + pw >= lx && px <= lx + lw;

    if (onPlatform) {
        character->timeOnLastPlatform += 1.0f / 60.0f; 
        if (character->timeOnLastPlatform >= 2.0f) {
            finished = true;
        }
    } else {
        character->timeOnLastPlatform = 0.0f;
    }
    std::cout << "MiniGameState :: Calculations completed" << std::endl;
}

void MiniGameState::resetCharacter(MinigameCharacterState* character) {
    std::cout << "MiniGameState :: Character reset" << std::endl;
    character->x = windowWidth - character->width;
    character->y = windowHeight - character->height;
    character->vx = 0.0f;
    character->vy = 0.0f;
    character->speed = 1.0f;
    character->facingRight = false;
    character->isFinished = false;
    character->isOnGround = true;
    character->timeOnLastPlatform = 0.0f;
}