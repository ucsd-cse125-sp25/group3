#include "Artifact.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Artifact::Artifact()
{
    artifact_init = true;
    artifact_id = 0;
}


Artifact::~Artifact() {
    for (AnInstance* i : movingInstances) {
        delete i;
    }
    for (AnInstance* i : initInstances) {
        delete i;
    }
}

void Artifact::addMovingInstance(AnInstance* instance) {
    movingInstances.push_back(instance);
}

void Artifact::addInitInstance(AnInstance* instance) {
    initInstances.push_back(instance);
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

void Artifact::setArtifactId(unsigned int id) {
    artifact_id = id;
}

void Artifact::update(AnimationPlayer* animationPlayer) {
    for (int i = 0; i < initInstances.size(); i++){
        if (i == artifact_id){
            if (artifact_init){
                initInstances[i]->update(animationPlayer);
            } else {
                std::cout << glm::to_string(baseModel) << std::endl;
                movingInstances[i]->setMMat(baseModel);
                movingInstances[i]->update(animationPlayer);
            }
        } else {
            initInstances[i]->update(animationPlayer);
        }
    }
}

void Artifact::draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager) {
    for (int i = 0; i < initInstances.size(); i++){
        if (i == artifact_id){
            if (artifact_init){
                // std::cout << "draw" << std::endl;
                initInstances[artifact_id]->draw(viewProjMtx, shaderManager);
            } else {
                // std::cout << "why" << std::endl;
                movingInstances[artifact_id]->draw(viewProjMtx, shaderManager);
            }
        } else {
            initInstances[i]->draw(viewProjMtx, shaderManager);
        }
    }
}

glm::vec3 Artifact::getPosition() const {
    return glm::vec3(baseModel[3]);
}
