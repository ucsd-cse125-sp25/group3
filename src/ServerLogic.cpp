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
    // for (int i=0; i<4; i++) {
    //     for (int j=0; j<4; j++) {
    //         printf("[%d,%d]: %f\n", i, j, baseModel[i][j]);
    //     }
    // }
}

void CubeState::toVector(std::vector<char>* vec) {
    char buf[4];

    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            // printf("elem i=%d, j=%d: %f\n", i,j,baseModel[i][j]);
            memcpy(buf, &baseModel[i][j], sizeof(float));
            vec->insert(vec->end(), &buf[0], &buf[4]);
        }
    }
    vec->resize(vec->size() + 1, isInvisible);
}

glm::vec3 CubeState::getPosition() {
    return glm::vec3(baseModel[3]);  // extract translation from matrix
}

// PlayerData::PlayerData() {
    
//     cube = new CubeState();
    
//     camera = new Camera();
// }

// PlayerData::~PlayerData() {
//     delete cube;
//     delete camera;
// }

void PlayerData::setCharacter(CharacterType character) {
    cube.type = character;
}

void PlayerData::calculateNewPos(KeyType key) {
    glm::vec3 forwardDir = camera.GetForwardVector();
    forwardDir.y = 0.0f;  
    forwardDir = glm::normalize(forwardDir);
    glm::vec3 rightDir = glm::normalize(glm::cross(forwardDir, glm::vec3(0, 1, 0)));
    glm::vec3 movement(0.0f);
    float speed = 0.02f;

    if (key == KeyType::KEY_W)
        movement += forwardDir;
    if (key == KeyType::KEY_S)
        movement -= forwardDir;
    if (key == KeyType::KEY_D)
        movement += rightDir;
    if (key == KeyType::KEY_A)
        movement -= rightDir;
        
    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement) * speed;
        cube.baseModel = glm::translate(cube.baseModel, movement);
    }

    // if (key == KeyType::KEY_T)
    //     cube.baseModel = glm::translate(cube.baseModel, glm::vec3(0, 0.005f, 0));

    // if (key == KeyType::KEY_G)
    //     cube.baseModel = glm::translate(cube.baseModel, glm::vec3(0, -0.005f, 0));

    // if (key == KeyType::KEY_K)
    //     cube.baseModel = glm::rotate(cube.baseModel, 0.02f, glm::vec3(0, 1, 0));

    // if (key == KeyType::KEY_L)
    //     cube.baseModel = glm::rotate(cube.baseModel, -0.02f, glm::vec3(0, 1, 0));
    
    if (key == KeyType::KEY_E) 
        if (!cube.eWasPressed) {
            switch (cube.type) {
                case CHARACTER_1: {
                    // Invisibility
                    if (!cube.isInvisible) {
                        cube.isInvisible = true;
                        cube.invisibleStartTime = std::chrono::steady_clock::now();
                    }
                    break;
                }
                case CHARACTER_2: {
                    // if (!cube.isSpeedBoosted) {
                    //     cube.isSpeedBoosted = true;
                    //     cube.speedBoostStartTime = std::chrono::steady_clock::now();
                    //     cube.speed = boostedSpeed;
                    // }
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

    if (cube.isInvisible && std::chrono::steady_clock::now() - cube.invisibleStartTime > std::chrono::seconds(cube.invisibleDuration)) {
        cube.isInvisible = false;
    }
    // if (isSpeedBoosted && std::chrono::steady_clock::now() - speedBoostStartTime > speedBoostDuration) {
    //     isSpeedBoosted = false;
    //     speed = normalSpeed;
    // }

    camera.Update(cube.getPosition()); 
}
