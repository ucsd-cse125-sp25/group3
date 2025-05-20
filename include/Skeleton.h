#pragma once

#include <vector>

#include "Joint.h"
#include "Tokenizer.h"
#include <assimp/scene.h>
#include "core.h"
#include <map>

class Skeleton {
private:
    Joint *root;
    std::map<std::string, Joint *> joints;

public:
    Skeleton();
    ~Skeleton();

    bool load(const aiNode* node);
    void log();
    void update();
    void draw(glm::mat4 viewProjMat, GLuint shader);
    void reset();

    int getJointNum();

    glm::mat4 getWorldMatrix(std::string name);
    void setPose(std::map<std::string, glm::mat4>& values);
    void setPoseRec(std::map<std::string, glm::mat4>& values);
};