#include "AnInstance.h"
#include "Model.h"
#include "AnimState.h"

AnInstance::AnInstance(Model* model){
    setState(AnimState::Stop);
    setModel(model);
    setMMat(glm::mat4(1.0f));
}

AnInstance::~AnInstance(){

}

void AnInstance::setModel(Model* model){
    jointMats.clear();
    
    this->model = model;
    int numMesh = model->getNumMeshes();
    jointMats.reserve(numMesh);
    for (int i = 0; i < numMesh; i++){
        jointMats[i].reserve(MAX_JOINTS);
        for (int i = 0; i < MAX_JOINTS; i++){
            jointMats[i].push_back(glm::mat4(1.0f));
        }
    }
}

void AnInstance::setMMat(glm::mat4 mMat){
    this->mMat = mMat;
}

/*
Sets animation state model will be animated with
*/

void AnInstance::setState(AnimState state){
    this->state = state;
    start_time = (float) glfwGetTime();
}

void AnInstance::update(AnimationPlayer* animPlayer){
    if (state != AnimState::Stop){
        animPlayer->setCurr(model->getModelType(), state);
        animPlayer->fullUpdate(start_time);
    }

    model->update(jointMats);
}

void AnInstance::draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager){
    model->draw(mMat, jointMats, viewProjMtx, shaderManager);
}