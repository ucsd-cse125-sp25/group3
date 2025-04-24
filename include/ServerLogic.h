#include "ClientLogic.h"
#include "core.h"
#include "Cube.h"
#include "Camera.h"
#include <cstdio>

class CubeState {
    public:
        glm::mat4 model;
        glm::vec3 color;

        // Cube Information
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        
        glm::mat4 baseModel = glm::mat4(1.0f);
        CubeState(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
        // ~CubeState();
};

class PlayerData {
    public:
        CubeState cube;
        Camera camera;
        // PlayerData();
        // ~PlayerData();
        void calculateNewPos(KeyType key);
};

void init();