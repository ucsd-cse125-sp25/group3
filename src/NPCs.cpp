#include "NPCs.h"
#include <iostream>

NPCs::NPCs(Character* character){
    npcModel = character;
    glm::vec3 startPos = glm::vec3(2.0f, 0.0f, -3.0f);
    glm::mat4 newmodel = glm::mat4(1.0f);
    newmodel = glm::translate(newmodel, startPos);

    // npcModel->setBaseModel(newmodel);
    npcModel->baseModel = newmodel;
    currentTarget = generateRandomTarget();

}


NPCs::~NPCs(){

}

void NPCs::draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager){
    // npcModel->setColor(glm::vec3(0.5f,0.7f,0.6f));
    npcModel->draw(viewProjMtx, shaderManager);
}

void NPCs::update(){
    
    // float random_num = static_cast <float> (rand())/ (static_cast <float> (RAND_MAX ));

    // // std::cout<< random_num << std::endl;
    // if (random_num < 0.5 ){
    //     // std::cout<< "stop" << std::endl;

    //     return;
    // }

    using namespace std::chrono;

    if (isWaiting) {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<duration<float>>(now - waitStartTime).count();

        if (elapsed >= waitDuration) {
            isWaiting = false;
            currentTarget = generateRandomTarget();  
        } else {
            return;  
        }
    }
    glm::vec3 currPos = npcModel->getPosition();
    glm::vec3 direction = currentTarget - currPos;

    if (glm::length(direction) < 0.05f) {
        float random_num = static_cast <float> (rand())/ (static_cast <float> (RAND_MAX ));
        if (random_num < 0.5 ){
            isWaiting = true;
        }
        waitStartTime = std::chrono::steady_clock::now();
        currentTarget = generateRandomTarget(); 
        return;
    }
    
    glm::vec3 movement = glm::normalize(direction) * speed;
    // npcModel->setBaseAndModel(glm::translate(npcModel->baseModel, movement));
    npcModel->baseModel = glm::translate(npcModel->baseModel, movement);
    npcModel->model->setMMat(npcModel->baseModel);
}

glm::vec3 NPCs::generateRandomTarget() {
    float x = -8.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (16.0f))); // [-8, 8]
    float z = -8.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (16.0f))); // [-8, 8]
    // std::cout<< x << "   " << z << std::endl;
    return glm::vec3(x, 0.0f, z);
}

void NPCs::updateFromPacket(const NPCPacket& packet, AnimationPlayer* animationPlayer) {
    glm::mat4 newModel;
    memcpy(glm::value_ptr(newModel), packet.model, sizeof(packet.model));
    // npcModel->setBaseModel(newModel);
    npcModel->model->setMMat(newModel);
    
    if (npcModel->model->getState() != packet.animState) {
        npcModel->model->setState(packet.animState);
    }
    npcModel->model->update(animationPlayer);
}
