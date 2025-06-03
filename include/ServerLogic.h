#include "ClientLogic.h"
#include "core.h"
#include "Cube.h"
#include "Camera.h"
#include <cstdio>
#include <chrono>
#include <set>
#include <map>
#include "AnimState.h"
#define TOTAL_PLAYERS 1

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
        glm::vec3 lastMoveDir = glm::vec3(0, 0, 1);

        //jumping
        bool isJumping = false;
        float jumpVelocity = 0.0f;
        float jumpHeight = 0.0f;
        float gravity = -0.00002f;
        float initialJumpVelocity = 0.015f;
        bool isGrounded = true;

        //for character abilities
        CharacterType type = NONE;  // default
        AnimState animState = AnimState::Stop;
        bool eWasPressed = false;

        // for invisible ability
        bool isInvisible = false;
        std::chrono::time_point<std::chrono::steady_clock> invisibleStartTime;
        int invisibleDuration = 5;
        
        //for speed boost ability
        float normalSpeed = 0.2f;
        float boostedSpeed = 0.5f;
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
        bool stateChanged = false;
        GameState currentState = INIT;
        CubeState cube;
        Camera camera;
        Camera miniMapCam;
        int windowWidth;
        int windowHeight;
        float lastX;
        float lastY;
        bool initialized = false;
        bool firstMouse;
        bool altDown;
        bool radarActive;
        std::chrono::steady_clock::time_point radarStartTime;
        int radarDuration = 5;

        // PlayerData();
        // ~PlayerData();
        bool init(InitPacket* packet);
        void calculateNewPos(KeyType key, ArtifactState* artifact);
        void handleCursor(double currX, double currY);
        void resetCamera();
        void saveToPacket(unsigned int client_id, InitPlayerPacket& packet);
        void updateRadarAbility();
        void update();

        void handleGuiInput(KeyType key);
};

class ServerLogic {
    public:
        static bool gameStarted;
        static bool availableChars[4];
        static bool processMovement(std::set<KeyType>& recievedMovementKeys, KeyType key);
        static void attemptGameStart(std::map<unsigned int, PlayerData*>& playersData);
};
