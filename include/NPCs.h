#pragma once

#include <vector>
#include "core.h"
#include "Cube.h"

class NPCs {
private:

public:

    NPCs(Cube* cube);
    ~NPCs();

    Cube* npcModel;

    enum State {
        IDLE,
        WALKING
    };
    State currentState;

    glm::vec3 nextPos;
    glm::vec3 currPos;

    void update();
    void draw(const glm::mat4& viewProjMtx, GLuint shader);

    glm::vec3 generatePosition();
};