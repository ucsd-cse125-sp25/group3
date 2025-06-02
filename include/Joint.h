#pragma once

#include <vector>
#include "Cube.h"
#include "core.h"
#include <assimp/scene.h>
#include "utils.h"
#include <map>

class Joint {
private:
    std::string name;

    glm::vec3 boxmin;
    glm::vec3 boxmax;
    glm::mat4 origMat;
    glm::mat4 worldMat, localMat;
    // glm::mat4 bindMat;
    Cube *cube;
    std::vector<Joint *> children;
    Joint *parent;

public:
    Joint();
    ~Joint();

    void update(const glm::mat4& parent);
    bool load(const aiNode* node);
    void addChild(Joint &j);
    void log(std::map<std::string, Joint *>& joints);
    void draw(glm::mat4 viewProjMat, GLuint shader);
    void resetDOFs();
    void resetDOFRecursive();

    void setPoseRec(std::map<std::string, glm::mat4>& values);
    void setPose(std::map<std::string, glm::mat4>& values);

    glm::mat4 getWorldMatrix();
};