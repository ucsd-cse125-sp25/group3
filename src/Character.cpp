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

Character::Character(AnInstance* model)
    : useModel(true), model(model)
{
    // model->setMMat(baseModel); 
    // baseModel = glm::scale(baseModel, glm::vec3(0.01f));
    model->setMMat(baseModel);
}


Character::~Character() {
    if (useModel) {
        delete model;
    } else {
        delete cube;
    }
}

void Character::update(AnimationPlayer* animationPlayer) {

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
        model->update(animationPlayer);
    } else {
        cube->baseModel = modelMatrix;
        cube->update();
    }
}

void Character::updateFromPacket(const InitPlayerPacket& packet, AnimationPlayer* animationPlayer) {
    if (model->getState() != packet.animState)
    model->setState(packet.animState);
    glm::mat4 modelMatrix;
    memcpy(glm::value_ptr(modelMatrix), packet.model, sizeof(packet.model));
    model->setMMat(modelMatrix);
    
    model->update(animationPlayer);
    isInvisible = packet.isInvisible;

    if (packet.currentState == IN_MINIGAME) {
        isInvisible = true;
    }
}

void Character::draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager) {
    if (isInvisible) return;

    if (useModel) {
        model->draw(viewProjMtx, shaderManager);
        // std::cout<< "yes use model draw" <<std::endl;
    } else {
        cube->draw(viewProjMtx, shaderManager->getShader(RenderMode::BASE, false), false);
    }
}

void Character::handleInput(GLFWwindow* window,
                 const glm::vec3& forwardDir,
                 const glm::vec3& rightDir,
                 const std::map<std::string, AABB>& museumAABBs,
                 const std::map<unsigned int, Character*>& otherPlayers,
                 const std::map<unsigned int, Character*> npcs) {
    // if (useModel) {
    //     model->handleInput(window, forwardDir, rightDir);
    // } else {
    //     cube->handleContinuousInput(window, forwardDir, rightDir);
    // }

    std::cout<< "yes inside handle input character  " <<std::endl;
    glm::vec3 movement(0.0f);
    float speed = 0.04f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement += forwardDir;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement -= forwardDir;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement += rightDir;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement -= rightDir;

    // if (glm::length(movement) > 0.0f) {
    //     movement = glm::normalize(movement) * speed;
    //     baseModel = glm::translate(baseModel, movement);

    //     lastMoveDir = glm::normalize(movement);
    // }

    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement) * speed;

        glm::vec3 nextPos = glm::vec3(baseModel[3]) + movement;
    
        float characterHalfSize = 0.3f; 
    
        AABB characterBox(
            nextPos - glm::vec3(characterHalfSize),
            nextPos + glm::vec3(characterHalfSize)
        );
    
        bool collided = false;
        for (const auto& [name, box] : museumAABBs) {
            if (box.intersects(characterBox)) {
                collided = true;
                std::cout<< "yes collide with museum " <<std::endl;
                break;
            }
        }

        // other player AABB
        for (const auto& [id, other] : otherPlayers) {
            if (other == this) continue; 
            glm::vec3 otherPos = other->getPosition();
            AABB otherBox(
                otherPos - glm::vec3(characterHalfSize),
                otherPos + glm::vec3(characterHalfSize)
            );
            if (characterBox.intersects(otherBox)) {
                std::cout<< "yes collide with other player  " <<std::endl;
                collided = true;
                break;
            }
        }

        // NPC AABB 
        if (!collided) {
            for (const auto& [id, npc] : npcs) {
                glm::vec3 npcPos = npc->getPosition();
                AABB npcBox(
                    npcPos - glm::vec3(characterHalfSize),
                    npcPos + glm::vec3(characterHalfSize)
                );
                if (characterBox.intersects(npcBox)) {
                    std::cout<< "yes collide with npc  " <<std::endl;
                    collided = true;
                    break;
                }
            }
        }

            
        if (!collided) {
            baseModel = glm::translate(baseModel, movement);
            lastMoveDir = glm::normalize(movement);
        } else {
            // std::cout << "Collision detected, movement blocked!" << std::endl;
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
