#pragma once

#include "core.h"
#include "Model.h"
#include "AnimState.h"
#include "AnimationPlayer.h"
#include "ShaderManager.h"

class AnInstance {
private:

    Model* model;

    /* model matrix instance is animated with */
    glm::mat4 mMat;

    // glm::mat4 VPMmMtx;

    /* The state the instance is animated in 
     stop = no animation, idle = currently also stop, walk = walks? */
    AnimState state;

    std::vector<std::vector<glm::mat4>> jointMats;

    /* Internal state of when we changed to this animation state */
    float start_time;

    void resetJointMats();

public:
    AnInstance(Model* model);

    ~AnInstance();

    void setModel(Model* model);

    void setMMat(glm::mat4 mMat);

    /*
    Sets animation state model will be animated with
    */

    void setState(AnimState state);

    AnimState getState();

    void update(AnimationPlayer* animPlayer);
    
    void draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager);

};