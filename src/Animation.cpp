#include "Animation.h"
#include <string>
#include <glm/gtx/string_cast.hpp>
 #include <iostream>
 #include "Skeleton.h"

Animation::Animation(){}

Animation::~Animation(){
    for (auto it = channels.begin(); it != channels.end(); ++it){
        Channel* ch = it->second;
        delete ch;
    }
}

bool Animation::load(const aiAnimation* animation){
    timeStart = 0.0f;
    timeEnd = (float) (animation->mDuration);
    ticksPerSec = (float) animation->mTicksPerSecond;

    int numChannels = animation->mNumChannels;
    for(int i = 0; i < numChannels; i++){

        Channel *channel = new Channel();
        channel->load(animation->mChannels[i]);
        channels[channel->getName()] = channel;
    }

    return true;
}

std::vector<std::string> Animation::getNames(){
    std::vector<std::string> keys;
    for (auto it = channels.begin(); it != channels.end(); ++it){
        keys.push_back(it->first);
    }
    return keys;
}

void Animation::evaluate(float time, std::map<std::string, glm::mat4>& pose){
    for (auto it = channels.begin(); it != channels.end(); ++it){
        Channel* ch = it->second;
        glm::mat4 val = ch->evaluate(time); 
        pose[ch->getName()] = val;
    }
}