#include "ClientLogic.h"
#include "core.h"
#include "Cube.h"
#include "Camera.h"
#include <cstdio>
#include <chrono>

//player info
enum CharacterType {
    CHARACTER_1,
    CHARACTER_2,
    CHARACTER_3,
    CHARACTER_4
};

enum ClientStatus {
    ONGOING,
    SUCCESS,
    DISCONNECT,
    
};

class CubeState {
    private:
        glm::mat4 model;

    public:
        glm::vec3 color;

        // Cube Information
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        
        glm::mat4 baseModel = glm::mat4(1.0f);

        //jumping
        bool isJumping = false;
        float jumpVelocity = 0.0f;
        float jumpHeight = 0.0f;
        float gravity = -0.00002f;
        float initialJumpVelocity = 0.015f;
        bool isGrounded = true;

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
        void update();
        void printState();
        glm::vec3 getPosition();
};

class PlayerData {
    public:
        CubeState cube;
        Camera camera;
        Camera miniMapCam;
        int windowWidth;
        int windowHeight;
        bool initialized = false;
        bool firstMouse;
        bool altDown;
        // PlayerData();
        // ~PlayerData();
        void init(char * data);
        void calculateNewPos(KeyType key);
        void setCharacter(CharacterType character);
        void toVector(unsigned int client_id, std::vector<char> * vec);
        void handleCursor(double currX, double currY);
        void resetCamera();
};

