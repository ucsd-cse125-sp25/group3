#include "Skeleton.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Skeleton::Skeleton() {}
Skeleton::~Skeleton() {
    delete root;
}

bool Skeleton::load(const aiNode* node) {
    // Parse tree
    root = new Joint();
    return (root -> load(node));
}

//call immediately after load
void Skeleton::log(){
    root -> log(joints);//depth first
}
void Skeleton::update() {
    root -> update(glm::mat4(1.0f)); //identity matrix is "parent" matrix
}
void Skeleton::draw(glm::mat4 viewProjMat, GLuint shader) {
    root -> draw(viewProjMat, shader);
}

void Skeleton::reset(){
    root -> resetDOFRecursive();
}

int Skeleton::getJointNum(){
    return joints.size();
}

void Skeleton::setPose(std::map<std::string, glm::mat4>& values){    
    // std::cout << glm::to_string(values["mixamorig:Spine1"]) << std::endl;
    for(auto it = joints.begin(); it != joints.end(); ++it){
        Joint* j = it->second;
        j->setPose(values);
    }
}

void Skeleton::setPoseRec(std::map<std::string, glm::mat4>& values){
    root -> setPoseRec(values);
}

glm::mat4 Skeleton::getWorldMatrix(std::string name){
    assert(joints.find(name) != joints.end());

    return joints[name]->getWorldMatrix();
}
