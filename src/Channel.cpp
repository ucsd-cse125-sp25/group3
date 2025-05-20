#include "Channel.h"
#include <string>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "utils.h"

Channel::Channel(){}

Channel::~Channel(){
}

std::string Channel::getName(){
    return name;
}

bool Channel::load(const aiNodeAnim* channel){
    this->name = channel->mNodeName.data;

    int numPos = channel->mNumPositionKeys;

    posKFs.reserve(numPos);

    for (int i = 0; i < numPos; i++){
        VecKF keyframe;
        keyframe.value = aiVecToGLM(channel->mPositionKeys[i].mValue);
        keyframe.time = (float) (channel->mPositionKeys[i].mTime);

        // std::cout << keyframe.time << std::endl;

        posKFs.push_back(keyframe);    
    }

    int numScale = channel->mNumScalingKeys;

    scaleKFs.reserve(numScale);

    for (int i = 0; i < numScale; i++){
        VecKF keyframe;
        keyframe.value = aiVecToGLM(channel->mScalingKeys[i].mValue);
        keyframe.time = (float) (channel->mScalingKeys[i].mTime);
        
        scaleKFs.push_back(keyframe);    
    }

    int numRot = channel->mNumRotationKeys;

    rotKFs.reserve(numRot);

    for (int i = 0; i < numRot; i++){
        QuatKF keyframe;
        keyframe.value = aiQuatToGLM(channel->mRotationKeys[i].mValue);
        keyframe.time = (float) (channel->mRotationKeys[i].mTime);
        
        rotKFs.push_back(keyframe);    
    }

    extrapI = CYCLE_E;
    extrapO = CYCLE_E;

    return true;
}

glm::vec3 Channel::evalPosRaw(float time){

    //case less than timemin
    float tMin = posKFs[0].time;
    float tMax = posKFs[posKFs.size()-1].time;
    glm::vec3 toReturn = glm::vec3(0.0f);
    float timeAdj = time;
    if (posKFs.size() == 0){
        return toReturn;
    }
    if (posKFs.size() == 1){
        return posKFs[0].value;
    }

    if (time < tMin){
        if (extrapI == ExtrapType::CONSTANT_E){
            return posKFs[0].value;
        }            

        float timeDelta = tMin - time; 
        timeDelta = abs(timeDelta);       
        float range = tMax - tMin;
        int numRep = (int)(timeDelta/range);
        //std::cout << "numrep" << numRep << std::endl;
        if (extrapI == ExtrapType::CYCLE_E){// || extrapI == ExtrapType::CYCLE_OFFSET_E)
            timeAdj = tMax-(timeDelta-(numRep*range)); // cycle
        }

    } else if (time > tMax){    //case greater than timemax
        if (extrapO == ExtrapType::CONSTANT_E){ 
            return posKFs[posKFs.size()-1].value;
        }            

        float timeDelta = time-tMax;
        timeDelta = abs(timeDelta);

        float range = tMax - tMin;
        int numRep = (int)(timeDelta/range);
        //std::cout << "numrep" << numRep << std::endl;
        if (extrapO == ExtrapType::CYCLE_E) {// || extrapO == ExtrapType::CYCLE_OFFSET_E)
            timeAdj = tMin + (timeDelta-(numRep*range)); // cycle
        }
    }
    //else find two keyframes where it is within (i and i+1)
    int t0 = -1;

    int l = 0;
    int r = posKFs.size()-1;
    
    //binary search if not near
    if (t0 == -1){
        while(l <= r) {
            t0 = l + (r - l)/2;

            if (posKFs[t0].time == timeAdj){
                toReturn += posKFs[t0].value;
                return toReturn;
            }

            if (posKFs[t0].time < timeAdj){
                if (posKFs[t0+1].time > timeAdj){
                    break;
                }
                l = t0 + 1;
            }

            if (posKFs[t0].time > timeAdj){
                r = t0 - 1;
            }
        }
    }
    
    if (l > r){
        std::cout << "AH" << std::endl;
    }

    float u = timeAdj - posKFs[t0].time;
    float denom = posKFs[t0+1].time - posKFs[t0].time;

    u = u/denom;

    //eval
    // glm::vec3 temp = u*posKFs[t0].A;
    // temp += posKFs[t0].B;
    // temp *= u;
    // temp += posKFs[t0].C;
    // temp *= u;
    // temp += posKFs[t0].D;
    // toReturn += temp;
    //currently only implementing lienar mix.
    toReturn = glm::mix(posKFs[t0].value, posKFs[t0+1].value, u);

    return toReturn;
}

glm::mat4 Channel::evalPos(float time){
    return glm::translate(glm::mat4(1.0f), evalPosRaw(time));
}

glm::vec3 Channel::evalScaleRaw(float time){

    //case less than timemin
    float tMin = scaleKFs[0].time;
    float tMax = scaleKFs[scaleKFs.size()-1].time;
    glm::vec3 toReturn = glm::vec3(0.0f);
    float timeAdj = time;
    if (scaleKFs.size() == 0){
        return toReturn;
    }
    if (scaleKFs.size() == 1){
        return scaleKFs[0].value;
    }

    if (time < tMin){
        if (extrapI == ExtrapType::CONSTANT_E){
            return scaleKFs[0].value;
        }            

        float timeDelta = tMin - time; 
        timeDelta = abs(timeDelta);       
        float range = tMax - tMin;
        int numRep = (int)(timeDelta/range);
        //std::cout << "numrep" << numRep << std::endl;
        if (extrapI == ExtrapType::CYCLE_E){// || extrapI == ExtrapType::CYCLE_OFFSET_E)
            timeAdj = tMax-(timeDelta-(numRep*range)); // cycle
        }

    } else if (time > tMax){    //case greater than timemax
        if (extrapO == ExtrapType::CONSTANT_E){ 
            return scaleKFs[scaleKFs.size()-1].value;
        }            

        float timeDelta = time-tMax;
        timeDelta = abs(timeDelta);

        float range = tMax - tMin;
        int numRep = (int)(timeDelta/range);
        //std::cout << "numrep" << numRep << std::endl;
        if (extrapO == ExtrapType::CYCLE_E) {// || extrapO == ExtrapType::CYCLE_OFFSET_E)
            timeAdj = tMin + (timeDelta-(numRep*range)); // cycle
        }
    }
    //else find two keyframes where it is within (i and i+1)
    int t0 = -1;

    int l = 0;
    int r = scaleKFs.size()-1;
    
    //binary search if not near
    if (t0 == -1){
        while(l <= r) {
            t0 = l + (r - l)/2;

            if (scaleKFs[t0].time == timeAdj){
                toReturn += scaleKFs[t0].value;
                return toReturn;
            }

            if (scaleKFs[t0].time < timeAdj){
                if (scaleKFs[t0+1].time > timeAdj){
                    break;
                }
                l = t0 + 1;
            }

            if (scaleKFs[t0].time > timeAdj){
                r = t0 - 1;
            }
        }
    }
    
    if (l > r){
        std::cout << "AH" << std::endl;
    }

    float u = timeAdj - scaleKFs[t0].time;
    float denom = scaleKFs[t0+1].time - scaleKFs[t0].time;

    u = u/denom;

    //eval
    // glm::vec3 temp = u*scaleKFs[t0].A;
    // temp += scaleKFs[t0].B;
    // temp *= u;
    // temp += scaleKFs[t0].C;
    // temp *= u;
    // temp += scaleKFs[t0].D;
    // toReturn += temp;
    //currently only implementing lienar mix.
    toReturn = glm::mix(scaleKFs[t0].value, scaleKFs[t0+1].value, u);

    return toReturn;
}

glm::mat4 Channel::evalScale(float time){
    return glm::scale(glm::mat4(1.0f), evalScaleRaw(time));
}

glm::quat Channel::evalRotRaw(float time){

    //case less than timemin
    float tMin = rotKFs[0].time;
    float tMax = rotKFs[rotKFs.size()-1].time;
    glm::quat toReturn = glm::vec3(0.0f);
    float timeAdj = time;
    if (rotKFs.size() == 0){
        return toReturn;
    }
    if (rotKFs.size() == 1){
        return rotKFs[0].value;
    }

    if (time < tMin){
        if (extrapI == ExtrapType::CONSTANT_E){
            return rotKFs[0].value;
        }            

        float timeDelta = tMin - time; 
        timeDelta = abs(timeDelta);       
        float range = tMax - tMin;
        int numRep = (int)(timeDelta/range);
        //std::cout << "numrep" << numRep << std::endl;
        if (extrapI == ExtrapType::CYCLE_E){// || extrapI == ExtrapType::CYCLE_OFFSET_E)
            timeAdj = tMax-(timeDelta-(numRep*range)); // cycle
        }

    } else if (time > tMax){    //case greater than timemax
        if (extrapO == ExtrapType::CONSTANT_E){ 
            return rotKFs[rotKFs.size()-1].value;
        }            

        float timeDelta = time-tMax;
        timeDelta = abs(timeDelta);

        float range = tMax - tMin;
        int numRep = (int)(timeDelta/range);
        //std::cout << "numrep" << numRep << std::endl;
        if (extrapO == ExtrapType::CYCLE_E) {// || extrapO == ExtrapType::CYCLE_OFFSET_E)
            timeAdj = tMin + (timeDelta-(numRep*range)); // cycle
        }
    }
    //else find two keyframes where it is within (i and i+1)
    int t0 = -1;

    int l = 0;
    int r = rotKFs.size()-1;
    
    //binary search if not near
    if (t0 == -1){
        while(l <= r) {
            t0 = l + (r - l)/2;

            if (rotKFs[t0].time == timeAdj){
                toReturn += rotKFs[t0].value;
                return toReturn;
            }

            if (rotKFs[t0].time < timeAdj){
                if (rotKFs[t0+1].time > timeAdj){
                    break;
                }
                l = t0 + 1;
            }

            if (rotKFs[t0].time > timeAdj){
                r = t0 - 1;
            }
        }
    }
    
    if (l > r){
        std::cout << "AH" << std::endl;
    }

    float u = timeAdj - rotKFs[t0].time;
    float denom = rotKFs[t0+1].time - rotKFs[t0].time;

    u = u/denom;

    //eval
    // glm::vec3 temp = u*rotKFs[t0].A;
    // temp += rotKFs[t0].B;
    // temp *= u;
    // temp += rotKFs[t0].C;
    // temp *= u;
    // temp += rotKFs[t0].D;
    // toReturn += temp;
    //currently only implementing lienar mix.
    toReturn = glm::slerp(rotKFs[t0].value, rotKFs[t0+1].value, u);

    toReturn = glm::normalize(toReturn);

    return toReturn;
}

glm::mat4 Channel::evalRot(float time){
    return glm::toMat4(evalRotRaw(time));
}

glm::mat4 Channel::evaluate(float time){
    return evalPos(time) * evalRot(time) * evalScale(time);
}