#include "AnimationPlayer.h"
#include <iostream>

AnimationPlayer::AnimationPlayer(){
}

AnimationPlayer::~AnimationPlayer(){}

void AnimationPlayer::setAnim(Animation* anim){
    this->anim = anim;
}

void AnimationPlayer::loadAnims(const std::string& file){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, 
        aiProcess_Triangulate);
    if (scene == nullptr){std::cout << "breh" << file << std::endl;}
    assert(scene && scene->mRootNode);

    skel = new Skeleton();
    skel->load(scene->mRootNode);
    skel->log();
    skel->update();

    anim = new Animation();
    anim->load(scene->mAnimations[0]);

    // for (int i = 0; i < scene->mNumAnimations; i++){
    //     std::cout << "new animation" << std::endl;
    //     anim = new Animation();
    //     anim->load(scene->mAnimations[i]);
    // }
}

void AnimationPlayer::initPose(){
    for(std::string name : anim->getNames()){
        pose[name] = glm::mat4(1.0f);
    }
    // anim -> evaluate(3.0f, pose);    
}

void AnimationPlayer::setSkel(Skeleton* skel){
    this->skel = skel;
}

Skeleton* AnimationPlayer::getSkel(){
    return skel;
}

void AnimationPlayer::draw(glm::mat4 viewProjMat, GLuint shader) {
    skel -> draw(viewProjMat, shader);
}

void AnimationPlayer::fullUpdate(){
    update();
    poseSkel();
    skel->update();
}

/*evaluates the local matrices for each channel*/
void AnimationPlayer::update(){
    time = (float) glfwGetTime();
    // std::cout << time << std::endl;
    anim -> evaluate(time, pose);
}

/*Sets the local matrices of the skeleton*/
void AnimationPlayer::poseSkel(){
    skel->setPose(pose);
}