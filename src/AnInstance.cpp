#include "AnInstance.h"
#include "Model.h"
#include "AnimState.h"
#include "glm/gtx/string_cast.hpp"

AnInstance::AnInstance(Model* model){
    setModel(model);
    setState(AnimState::Stop);
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
        std::vector<glm::mat4> jm;
        jm.reserve(MAX_JOINTS);
        for (int j = 0; j < MAX_JOINTS; j++){
            jm.push_back(glm::mat4(1.0f));
        }
        jointMats.push_back(jm);
    }
}

void AnInstance::resetJointMats(){
    for (int i = 0; i < jointMats.size(); i++){
        for (int j = 0; j < jointMats[i].size(); j++){
            jointMats[i][j] = glm::mat4(1.0f);
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
    if (state == AnimState::Stop){
        resetJointMats();
    }
}

AnimState AnInstance::getState(){
    return state;
}

void AnInstance::update(AnimationPlayer* animPlayer){
    // std::cout << "up start" << std::endl;
    if (state != AnimState::Stop){
        animPlayer->setCurr(model->getModelType(), state);
        animPlayer->fullUpdate(start_time);
        
        model->update(jointMats);
    }
    // std::cout << "up middle" << std::endl;

    // std::cout << glm::to_string(jointMats[0][0]) << std::endl;

    // std::cout << "up end" << std::endl;
}

void AnInstance::draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager){
    // std::cout << "draw start" << std::endl;
    glm::mat4 VPMmMtx = viewProjMtx*mMat;
    model->draw(jointMats, VPMmMtx, shaderManager);
    // std::cout << "draw end" << std::endl;
}