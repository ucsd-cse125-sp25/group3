#include "Joint.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Joint::Joint() {
    boxmin = glm::vec3(-1.0f, -0.5f, -0.5f);
    boxmax = glm::vec3(1.0f, 0.5f, 0.5f);

    cube = new Cube(boxmin, boxmax);

    origMat = glm::mat4(1.0f);
    localMat = origMat;
}

Joint::~Joint() {
    delete cube;
    for(Joint *child : children) {
        child -> ~Joint();
        delete child;
    }
}

void Joint::update(glm::mat4& parent) {
    // compute world matrix W
    worldMat = parent*localMat;

    // recursively call Update() on children
    for(Joint *child : children){
        child -> update(worldMat);
    }
}

bool Joint::load(const aiNode* node) {
    assert(node);

    this->name = node->mName.data;
    this->origMat = aiMatToGLM(node->mTransformation);
    localMat = origMat;

    int numChild = node->mNumChildren;

    children.reserve(numChild);
    
    for (int i = 0; i < numChild; i++){
        Joint *jnt = new Joint();
        jnt->load(node->mChildren[i]);
        addChild(*jnt);
    }

    return true;
}

void Joint::addChild(Joint &j) {
    children.push_back(&j);
}

void Joint::log(std::map<std::string, Joint *>& joints){
    joints[name] = this;
    for (Joint* child : children){
        child -> log(joints);
    }
}

void Joint::draw(glm::mat4 viewProjMat, GLuint shader) { 
    // draw oriented box with opengl (use cube)
    // take what cube has? okay, so window creates skeleton using file main received via command line, then skeleton's draw is called like cube's draw, and then skeleton's draw calls joint's draw, which calls cube draw.
    //cube->draw(viewProjMat, shader);
    if (name == "mixamorig:Spine1"){
        std::cout << glm::to_string(worldMat) << std::endl;
        cube->draw(viewProjMat * worldMat, shader, false);
    }
    // recursively call Draw() on children
    for(Joint *child : children) {
        child -> draw(viewProjMat, shader);
    }
}

void Joint::resetDOFs(){
    localMat = origMat;
}

void Joint::resetDOFRecursive(){
    resetDOFs();
    for(Joint *child : children) {
        child -> resetDOFRecursive();
    }
}

glm::mat4 Joint::getWorldMatrix(){
    //std::cout << "JointLM: " << glm::to_string(localMat) << std::endl;
    //std::cout << "JointWM: " << glm::to_string(worldMat) << std::endl;
    return worldMat;
}

void Joint::setPoseRec(std::map<std::string, glm::mat4>& values){

    if (values.find(name) == values.end()){
        localMat = origMat;
    } else {
        localMat = values[name];
    }

    for(Joint *child : children) {
        child -> setPoseRec(values);
    }
}

void Joint::setPose(std::map<std::string, glm::mat4>& values){
    // assert(values.find(name) != values.end());
    if (values.find(name) == values.end()){
        localMat = origMat;
    } else {
        // std::cout << name << std::endl;
        localMat = values[name];
    }
}