#include "ClientLogic.h"
#include "core.h"
#include "Cube.h"
#include "Camera.h"
#include <cstdio>
#include <chrono>

enum ClientStatus {
    ONGOING,
    SUCCESS,
    DISCONNECT,
};

class CubeState {
    public:
        glm::vec3 color;

        // Cube Information
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        glm::mat4 model;
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
        void update();
        void printState();
        glm::vec3 getPosition();

        void updateFromPacket(const InitPlayerPacket& packet);
        void saveToPacket(InitPlayerPacket& packet);
};

class NPCState {
    private:
        glm::vec3 pointA = glm::vec3(-3.0f, 0.0f, 0.0f);
        glm::vec3 pointB = glm::vec3(3.0f, 0.0f, 0.0f);
        bool goingToA = false; 
        float speed = 0.03f;

        bool isWaiting = false;
        std::chrono::steady_clock::time_point waitStartTime;
        float waitDuration = 2.0f; 

    public:

        NPCState();

        CubeState npcModel;

        enum State {
            IDLE,
            WALKING
        };
        State currentState;

        glm::vec3 nextPos;
        glm::vec3 currPos;
        glm::vec3 currentTarget;

        void update();
        glm::vec3 generatePosition();
        glm::vec3 generateRandomTarget();
        void saveToPacket(NPCPacket& packet);
};

class ArtifactState {
    public:
        CubeState* holder;
        CubeState artifactModel;
        ArtifactState();
        void update();
        void attemptGrab(CubeState * player);
        void saveToPacket(StateUpdatePacket& packet);
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
        bool radarActive;
        std::chrono::steady_clock::time_point radarStartTime;
        int radarDuration = 5;

        // PlayerData();
        // ~PlayerData();
        void init(InitPacket* packet);
        void calculateNewPos(KeyType key, ArtifactState* artifact);
        void handleCursor(double currX, double currY);
        void resetCamera();
        void saveToPacket(unsigned int client_id, InitPlayerPacket& packet);
        void updateRadarAbility();
        void update();
};

// class ServerLogic {
//     public:
//         CubeState artifact;
//         static void updateArtifact();

// };
