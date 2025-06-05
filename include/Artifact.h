#pragma once

#include "AnInstance.h"
#include "ShaderManager.h"
#include "AnimationPlayer.h"
#include "../packets/InitPlayerPacket.h"
#include "AABB.h"

class Artifact {
private:

public:
    std::vector<AnInstance*> movingInstances;
    std::vector<AnInstance*> initInstances;
    bool artifact_init;
    unsigned int artifact_id;          
    glm::mat4 baseModel = glm::mat4(1.0f);  

    Artifact();  
    ~Artifact();

    void setArtifactId(unsigned int id);

    void addMovingInstance(AnInstance* instance);

    void addInitInstance(AnInstance* instance);

    void setArtifactState(unsigned int state);

    void setBaseModel(glm::mat4 model);

    void update(AnimationPlayer* animationPlayer);
    
    void draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager);

    glm::vec3 getPosition() const;
};