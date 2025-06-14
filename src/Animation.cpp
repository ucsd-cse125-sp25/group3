#include "Animation.h"
#include <string>
#include <glm/gtx/string_cast.hpp>
 #include <iostream>
 #include "Skeleton.h"

Animation::Animation(){
    duration = 0.0f;
}

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
    duration = ((float) animation->mDuration) / ticksPerSec;
    std::cout << ticksPerSec << std::endl;
    std::cout << timeEnd << std::endl;

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
    float frameTime = time * ticksPerSec;
    // std::cout << frameTime << std::endl;
    for (auto it = channels.begin(); it != channels.end(); ++it){
        Channel* ch = it->second;
        glm::mat4 val = ch->evaluate(frameTime); 
        pose[ch->getName()] = val;
    }
}

float Animation::getDuration() {
    return duration;
}
