#include "Character.h"
#include <glm/gtc/matrix_transform.hpp>

Character::Character(bool useModel, const std::string& modelPath)
    : useModel(useModel)
{
    if (useModel) {
        model = new Model();
        model->load(modelPath);
        model->setMMat(baseModel);
        model->setupBuf();
    } else {
        cube = new Cube();
        cube->baseModel = baseModel;
    }
}

Character::~Character() {
    if (useModel) {
        delete model;
    } else {
        delete cube;
    }
}

void Character::update() {
    
}

void Character::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    
}

void Character::handleInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir) {
    
}

void Character::triggerJump() {
    
}

glm::vec3 Character::getPosition() const {
    return glm::vec3(baseModel[3]);
}
