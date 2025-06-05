#pragma once

#include "AnInstance.h"
#include "ShaderManager.h"
#include "AnimationPlayer.h"
#include "../packets/InitPlayerPacket.h"
#include "AABB.h"

class Artifact {
private:

public:
    AnInstance* movingInstance;
    AnInstance* initInstance;
    bool artifact_init;          
    glm::mat4 baseModel = glm::mat4(1.0f);  

    Artifact();  
    ~Artifact();

    void addMovingInstance(AnInstance* instance);

    void addInitInstance(AnInstance* instance);

    void setArtifactState(unsigned int state);

    void setBaseModel(glm::mat4 model);

    void update(AnimationPlayer* animationPlayer);
    
    void draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager);

    glm::vec3 getPosition() const;
};