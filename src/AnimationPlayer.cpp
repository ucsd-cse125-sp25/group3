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

bool AnimationPlayer::loadAnims(ModelType modelType, std::map<std::string, AnimState> import, const std::string& file){
    
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
        delete skel;
        return false;
    }
    skel->log();
    skel->update();
    
    for (int i = 0; i < scene->mNumAnimations; i++){
        aiString name = scene->mAnimations[i]->mName;

        std::cout << name.C_Str() << std::endl;

        if (import.find(name.C_Str()) != import.end()){
            std::cout << "importing" << std::endl;

            AnimState state = import[name.C_Str()];

            Animation* anim = new Animation();

            if (!anim->load(scene->mAnimations[i])){
                delete anim;
                return false;
            }

            setAnim(modelType, state, anim);
        }
    }

    setSkel(modelType, skel);

    return true;
    // for (int i = 0; i < scene->mNumAnimations; i++){
    //     std::cout << "new animation" << std::endl;
    //     anim = new Animation();
    //     anim->load(scene->mAnimations[i]);
    // }
}

void AnimationPlayer::setCurr(ModelType modelType, AnimState animState){
    
    // These will throw an error if you try to access an animation that doesn't exist
    assert(animMap.find(modelType) != animMap.end());
    assert(animMap[modelType].find(animState) != animMap[modelType].end());
    assert(skelMap.find(modelType) != skelMap.end());

    currAnim = animMap[modelType][animState];
    pose.clear();
    for(std::string name : currAnim->getNames()){
        pose[name] = glm::mat4(1.0f);
    }
    currSkel = skelMap[modelType];
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
    // std::cout << "pose start" << std::endl;
    poseSkel();
    // std::cout << "pose done" << std::endl;
    currSkel->update();
    // std::cout << "update done" << std::endl;
}

/*evaluates the local matrices for each channel*/
void AnimationPlayer::update(float start_time){
    time = (float) glfwGetTime();
    time = time - start_time;
    // std::cout << time << std::endl;
    currAnim -> evaluate(time, pose);
    // std::cout << "evaldone" << std::endl;
}

/*Sets the local matrices of the skeleton*/
void AnimationPlayer::poseSkel(){
    currSkel->setPose(pose);
}