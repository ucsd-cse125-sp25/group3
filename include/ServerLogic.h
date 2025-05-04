#include "ClientLogic.h"
#include "core.h"
#include "Cube.h"
#include "Camera.h"
#include <cstdio>

//player info
enum CharacterType {
    CHARACTER_1,
    CHARACTER_2,
    CHARACTER_3,
    CHARACTER_4
};

class CubeState {
    public:
        glm::mat4 model;
        glm::vec3 color;

        // Cube Information
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        
        glm::mat4 baseModel = glm::mat4(1.0f);

        //for character abilities
        CharacterType type = CHARACTER_1;  // default
        bool eWasPressed = false;

        // for invisible ability
        bool isInvisible = false;
        std::chrono::time_point<std::chrono::steady_clock> invisibleStartTime;
        int invisibleDuration = 5;
        
        //for speed boost ability
        float normalSpeed = 0.02f;
        float boostedSpeed = 0.06f;
        float speed = normalSpeed;
        bool isSpeedBoosted = false;
        std::chrono::time_point<std::chrono::steady_clock> speedBoostStartTime;
        int speedBoostDuration = 5;

        CubeState(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
        void toVector(std::vector<char>* vec);
        void printState();
        glm::vec3 getPosition();
};

class PlayerData {
    public:
        CubeState cube;
        Camera camera;
        // PlayerData();
        // ~PlayerData();
        void calculateNewPos(KeyType key);
        void setCharacter(CharacterType character);
};

void init();