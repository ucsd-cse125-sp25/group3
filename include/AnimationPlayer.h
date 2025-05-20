#pragma once

#include <vector>
#include "Animation.h"
#include "Skeleton.h"
#include "core.h"
#include <map>
#include "CharState.h"

class AnimationPlayer {
private:
    Animation* anim;
    Skeleton* skel;
    double time;
    std::map<std::string, glm::mat4> pose;

public:
    AnimationPlayer();
    ~AnimationPlayer();
    
    //loads in a new animation and skeleton from a file.
    void loadAnims(const std::string& file);
    void setAnim(Animation* anim);
    void setSkel(Skeleton* skel);

    Skeleton* getSkel();

    //call after anim is set
    void AnimationPlayer::initPose();

    void AnimationPlayer::draw(glm::mat4 viewProjMat, GLuint shader);

    void fullUpdate();
    void update();
    void poseSkel();
};