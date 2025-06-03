#pragma once

#include <vector>
#include "Tokenizer.h"
#include "Channel.h"
#include <iostream>
#include "core.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

class Animation {
private:
    float duration;
    float timeStart, timeEnd;
    float ticksPerSec;
    std::map<std::string, Channel*> channels;
public:
    Animation();
    ~Animation();
    bool load(const aiAnimation* animation);
    std::vector<std::string> getNames();
    void evaluate(float time, std::map<std::string, glm::mat4>& pose);
    /*
    Returns the duration of the animation (1 cycle of it)
    */
    float getDuration();
};