#pragma once

#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <iostream>
#include "core.h"
#include <assimp/scene.h>

enum ExtrapType {CONSTANT_E, CYCLE_E};
// enum TangentMode {FLAT_T, LINEAR_T, SMOOTH_T, FIXED_T};

struct VecKF {
    float time;
    glm::vec3 value;
    // float tangentIn, tangentOut;
    // enum TangentMode tanMI, tanMO;
    // float A, B, C, D; //cubic coefficients
};

struct QuatKF {
    float time;
    glm::quat value;
    // float tangentIn, tangentOut;
    // enum TangentMode tanMI, tanMO;
    // float A, B, C, D;
};

class Channel {
private:
    std::string name;

    ExtrapType extrapI, extrapO;
    std::vector<VecKF> posKFs;
    std::vector<VecKF> scaleKFs;
    std::vector<QuatKF> rotKFs;

    glm::vec3 evalPosRaw(float time);
    glm::quat evalRotRaw(float time);
    glm::vec3 evalScaleRaw(float time);

    glm::mat4 evalPos(float time);
    glm::mat4 evalRot(float time);
    glm::mat4 evalScale(float time);
public:
    Channel();
    ~Channel();
    bool load(const aiNodeAnim* channel);

    std::string getName();

    glm::mat4 evaluate(float time);
};