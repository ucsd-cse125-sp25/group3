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
    baseModel = glm::translate(baseModel, glm::vec3(0.0f, 0.0f, -5.0f));
    baseModel = glm::scale(baseModel, glm::vec3(0.01f));
    // baseModel = glm::rotate(baseModel, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model->setMMat(baseModel);

    characterAABB_local = AABB::fromScene(model->getAiScene());

}


Character::~Character() {
    if (useModel) {
        delete model;
    } else {
        delete cube;
    }
}

void Character::update() {
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
        // std::cout<< "yes use model draw" <<std::endl;
    } else {
        cube->draw(viewProjMtx, shader, false);
    }
}

bool isColliding(const AABB& a, const AABB& b) {
    return (
        a.max.x > b.min.x && a.min.x < b.max.x &&
        a.max.y > b.min.y && a.min.y < b.max.y &&
        a.max.z > b.min.z && a.min.z < b.max.z
    );
}


AABB transformAABB(const AABB& box, const glm::mat4& M) {
    glm::vec3 corners[8] = {
        {box.min.x, box.min.y, box.min.z},
        {box.max.x, box.min.y, box.min.z},
        {box.max.x, box.max.y, box.min.z},
        {box.min.x, box.max.y, box.min.z},
        {box.min.x, box.min.y, box.max.z},
        {box.max.x, box.min.y, box.max.z},
        {box.max.x, box.max.y, box.max.z},
        {box.min.x, box.max.y, box.max.z}
    };

    AABB result;
    for (int i = 0; i < 8; ++i) {
        glm::vec4 worldPos = M * glm::vec4(corners[i], 1.0f);
        result.expandToInclude(glm::vec3(worldPos));
    }

    return result;
}


void Character::handleInput(GLFWwindow* window, const glm::vec3& forwardDir, 
        const glm::vec3& rightDir, const std::unordered_map<std::string, AABB>& sceneAABBs) {
    // if (useModel) {
    //     model->handleInput(window, forwardDir, rightDir);
    // } else {
    //     cube->handleContinuousInput(window, forwardDir, rightDir);
    // }
    glm::vec3 movement(0.0f);
    float speed = 0.8f;

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
        // baseModel = glm::translate(baseModel, movement);

        // lastMoveDir = glm::normalize(movement);
        glm::mat4 futureModel = glm::translate(baseModel, movement);
        AABB futureBox = transformAABB(characterAABB_local, futureModel);

        bool collided = false;
        for (const auto& [name, sceneBox] : sceneAABBs) {
            if (isColliding(futureBox, sceneBox)) {
                collided = true;
                break;
            }
        }

        if (!collided) {
            baseModel = futureModel;
            lastMoveDir = glm::normalize(movement);
        } else {
            // std::cout << "collision " << movement.x << std::endl; 
        }
    }

    // if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    //     baseModel = glm::translate(baseModel, glm::vec3(0, 0.005f, 0));
    // if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    //     baseModel = glm::translate(baseModel, glm::vec3(0, -0.005f, 0));
    // if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    //     baseModel = glm::rotate(baseModel, 0.02f, glm::vec3(0, 1, 0));
    // if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    //     baseModel = glm::rotate(baseModel, -0.02f, glm::vec3(0, 1, 0));

    // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded) {
    //     triggerJump();
    // }

    
    
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


AABB Character::getBoundingBox() const {
    return transformAABB(characterAABB_local, baseModel);
}
