#pragma once

#include <vector>
#include "Animation.h"
#include "Skeleton.h"
#include "core.h"
#include <map>
#include <unordered_map>
#include "ModelType.h"
#include "AnimState.h"
#include <string>

class AnimationPlayer {
private:
    std::unordered_map<ModelType, std::unordered_map<AnimState, Animation*>> animMap;
    std::unordered_map<ModelType, Skeleton*> skelMap;
    float time;
    std::map<std::string, glm::mat4> pose;
    Animation* currAnim;
    Skeleton* currSkel;

public:
    AnimationPlayer();
    ~AnimationPlayer();
    
    //loads in a new animation and skeleton from a file.
    bool loadAnims(ModelType modelType, std::map<std::string, AnimState> import, const std::string& file);
    void setAnim(ModelType modelType, AnimState animState, Animation* anim);
    void setSkel(ModelType modelType, Skeleton* skel);

    Skeleton* getSkel(ModelType modelType);

    //call after anim is set
    void setCurr(ModelType modelType, AnimState animState);

    // void AnimationPlayer::draw(glm::mat4 viewProjMat, GLuint shader);

    void fullUpdate(float start_time);
    void update(float start_time);
    void poseSkel();
};