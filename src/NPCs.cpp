#include "NPCs.h"
#include <iostream>

NPCs::NPCs(Cube* cube){
    npcModel = cube;
    glm::vec3 startPos = glm::vec3(2.0f, 0.0f, -3.0f);
    glm::mat4 newmodel = glm::mat4(1.0f);
    newmodel = glm::translate(newmodel, startPos);

    npcModel->setBaseModel(newmodel);
}


NPCs::~NPCs(){

}

void NPCs::draw(const glm::mat4& viewProjMtx, GLuint shader){
    npcModel->setColor(glm::vec3(0.5f,0.7f,0.6f));
    npcModel->draw(viewProjMtx,shader,false);
}

void NPCs::update(){

}
