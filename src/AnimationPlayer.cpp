#include "AnimationPlayer.h"
#include <iostream>

AnimationPlayer::AnimationPlayer(){
}

AnimationPlayer::~AnimationPlayer(){
    for(auto it = animMap.begin(); it != animMap.end(); ++it){
        std::unordered_map<AnimState, Animation*> aMap = it->second;
        for(auto it2 = aMap.begin(); it2 != aMap.end(); ++it2){
            delete it2->second;
        }
    }

    for(auto it = skelMap.begin(); it != skelMap.end(); ++it){
        delete it->second;
    }
}

void AnimationPlayer::setAnim(ModelType modelType, AnimState animState, Animation* anim){
    if (animMap.find(modelType) == animMap.end()){
        std::unordered_map<AnimState, Animation*> aMap;
        animMap[modelType] = aMap;
    }
    animMap[modelType][animState] = anim;
    // this->anim = anim;
}

bool AnimationPlayer::loadAnims(ModelType modelType, AnimState animState, const std::string& file){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, 
        aiProcess_Triangulate);
    if (scene == nullptr){
        std::cout << "breh" << file << std::endl;
        return false;
    }
    assert(scene && scene->mRootNode);

    Skeleton* skel = new Skeleton();
    if (!skel->load(scene->mRootNode)){
        return false;
    }
    skel->log();
    skel->update();

    Animation* anim = new Animation();
    if (!anim->load(scene->mAnimations[0])){
        return false;
    }


    setAnim(modelType, animState, anim);
    setSkel(modelType, skel);

    return true;
    // for (int i = 0; i < scene->mNumAnimations; i++){
    //     std::cout << "new animation" << std::endl;
    //     anim = new Animation();
    //     anim->load(scene->mAnimations[i]);
    // }
}

bool AnimationPlayer::setCurr(ModelType modelType, AnimState animState){
    
    currAnim = animMap[modelType][animState];
    pose.clear();
    for(std::string name : currAnim->getNames()){
        pose[name] = glm::mat4(1.0f);
    }
    // anim -> evaluate(3.0f, pose);    
}

void AnimationPlayer::setSkel(ModelType modelType, Skeleton* skel){
    skelMap[modelType] = skel;
}

Skeleton* AnimationPlayer::getSkel(ModelType modelType){
    if (skelMap.find(modelType) != skelMap.end()){
        return skelMap[modelType];
    } else {
        return nullptr;
    }
}

// void AnimationPlayer::draw(glm::mat4 viewProjMat, GLuint shader) {
//     skel -> draw(viewProjMat, shader);
// }

void AnimationPlayer::fullUpdate(float start_time){
    update(start_time);
    poseSkel();
    currSkel->update();
}

/*evaluates the local matrices for each channel*/
void AnimationPlayer::update(float start_time){
    time = (float) glfwGetTime();
    time = time - start_time;
    // std::cout << time << std::endl;
    currAnim -> evaluate(time, pose);
}

/*Sets the local matrices of the skeleton*/
void AnimationPlayer::poseSkel(){
    currSkel->setPose(pose);
}