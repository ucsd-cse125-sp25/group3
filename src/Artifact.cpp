#include "Artifact.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Artifact::Artifact()
{
    artifact_init = true;
    movingInstance = nullptr;
    initInstance = nullptr;
}


Artifact::~Artifact() {
    if (movingInstance != nullptr) {
        delete movingInstance;
    }
    if (initInstance != nullptr) {
        delete initInstance;
    }
}

void Artifact::addMovingInstance(AnInstance* instance) {
    movingInstance = instance;
}

void Artifact::addInitInstance(AnInstance* instance) {
    initInstance = initInstance;
}

void Artifact::setArtifactState(unsigned int state) {
    if (state == 1) {
        artifact_init = true;
    } else {
        artifact_init = false;
    }
}

void Artifact::setBaseModel(glm::mat4 model) {
    baseModel = model;
}


void Artifact::update(AnimationPlayer* animationPlayer) {
    if (artifact_init){
        initInstance->update(animationPlayer);
    } else {
        movingInstance->setMMat(baseModel);
        movingInstance->update(animationPlayer);
    }
}

void Artifact::draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager) {
    if (artifact_init){
        initInstance->draw(viewProjMtx, shaderManager);
    } else {
        movingInstance->draw(viewProjMtx, shaderManager);
    }
}

glm::vec3 Artifact::getPosition() const {
    return glm::vec3(baseModel[3]);
}
