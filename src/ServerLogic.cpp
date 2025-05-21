#include "ServerLogic.h"

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
    return glm::vec3(baseModel[3]);  // extract translation from matrix
}

void CubeState::updateFromPacket(const StateUpdatePacket& packet) {
    memcpy(&baseModel, packet.model, sizeof(packet.model));
    isInvisible = packet.isInvisible;
    printState();
}

// PlayerData::PlayerData() {
    
//     cube = new CubeState();
    
//     camera = new Camera();
// }

// PlayerData::~PlayerData() {
//     delete cube;
//     delete camera;
// }
void PlayerData::calculateNewPos(KeyType key) {
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
                    
                    break;
                }
            }
            cube.eWasPressed = true;
        } else {
            cube.eWasPressed = false;
        }
    }
}

void PlayerData::init(InitPacket* packet) {
    firstMouse = true;
    altDown = false;
    miniMapCam.SetOrtho(-10, 10, -10, 10, 0.1f, 100.0f); 
    miniMapCam.SetLookAt(glm::vec3(0, 20, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    cube.type = packet->character;
    windowWidth = packet->windowWidth;
    windowHeight = packet->windowHeight;
    printf("character: %d, width: %d, height: %d\n", cube.type, windowWidth, windowHeight);
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
    
    cube.update();
    camera.Update(cube.getPosition()); 
}

void PlayerData::resetCamera() {
    camera.Reset();
    camera.SetAspect(float(windowWidth) / float(windowHeight));
}