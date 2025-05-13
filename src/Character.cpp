#include "Character.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Character::Character(bool useModel, const std::string& modelPath)
//     : useModel(useModel)
// {
//     if (useModel) {
//         model = new Model();
//         model->load(modelPath);
//         model->setMMat(baseModel);
//         model->setupBuf();
//     } else {
//         cube = new Cube();
//         cube->baseModel = baseModel;
//     }
// }

Character::Character(Model* model)
    : useModel(true), model(model)
{
    // model->setMMat(baseModel);  // 设置初始位置
    baseModel = glm::scale(baseModel, glm::vec3(0.01f));
    model->setMMat(baseModel);
}


Character::~Character() {
    if (useModel) {
        delete model;
    } else {
        delete cube;
    }
}

void Character::update() {
    // 跳跃逻辑
    if (isJumping) {
        jumpVelocity += gravity;
        jumpHeight += jumpVelocity;
        if (jumpHeight <= 0.0f) {
            jumpHeight = 0.0f;
            jumpVelocity = 0.0f;
            isJumping = false;
            isGrounded = true;
        }
    }

    // // 生成变换矩阵
    // // glm::mat4 modelMatrix = glm::translate(baseModel, glm::vec3(0.0f, jumpHeight, 0.0f));
    

    glm::mat4 rotateM = glm::inverse(glm::lookAt(glm::vec3(0), lastMoveDir, glm::vec3(0, 1, 0)));

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, jumpHeight, 0.0f)) * baseModel;
    modelMatrix = modelMatrix * rotateM;
    // glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, jumpHeight, 0.0f)) * baseModel;


    if (useModel) {
        model->setMMat(modelMatrix);
        model->update();
    } else {
        cube->baseModel = modelMatrix;
        cube->update();
    }
}


void Character::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    if (useModel) {
        model->draw(viewProjMtx, shader);
        std::cout<< "yes use model draw" <<std::endl;
    } else {
        cube->draw(viewProjMtx, shader, false);
    }
}

void Character::handleInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir) {
    // if (useModel) {
    //     model->handleInput(window, forwardDir, rightDir);
    // } else {
    //     cube->handleContinuousInput(window, forwardDir, rightDir);
    // }
    glm::vec3 movement(0.0f);
    float speed = 0.5f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement += forwardDir;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement -= forwardDir;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement += rightDir;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement -= rightDir;

    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement) * speed;
        baseModel = glm::translate(baseModel, movement);

        lastMoveDir = glm::normalize(movement);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        baseModel = glm::translate(baseModel, glm::vec3(0, 0.005f, 0));
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        baseModel = glm::translate(baseModel, glm::vec3(0, -0.005f, 0));
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        baseModel = glm::rotate(baseModel, 0.02f, glm::vec3(0, 1, 0));
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        baseModel = glm::rotate(baseModel, -0.02f, glm::vec3(0, 1, 0));

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded) {
        triggerJump();
    }

    
    
}

void Character::triggerJump() {
    if (isGrounded) {
        isJumping = true;
        isGrounded = false;   
        jumpVelocity = initialJumpVelocity;
    }

}

glm::vec3 Character::getPosition() const {
    return glm::vec3(baseModel[3]);
}
