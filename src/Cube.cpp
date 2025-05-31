#include "Cube.h"

Cube::Cube(glm::vec3 cubeMin, glm::vec3 cubeMax) {
    // Model matrix.
    model = glm::mat4(1.0f);

    // The color of the cube. Try setting it to something else!
    color = glm::vec3(1.0f, 0.95f, 0.1f);

    // Specify vertex positions
    positions = {
        // Front
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),

        // Back
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),

        // Top
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

        // Bottom
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),

        // Left
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

        // Right
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z)};

    // Specify normals
    normals = {
        // Front
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),

        // Back
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),

        // Top
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),

        // Bottom
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),

        // Left
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),

        // Right
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0)};

    // Specify indices
    indices = {
        0, 1, 2, 0, 2, 3,        // Front
        4, 5, 6, 4, 6, 7,        // Back
        8, 9, 10, 8, 10, 11,     // Top
        12, 13, 14, 12, 14, 15,  // Bottom
        16, 17, 18, 16, 18, 19,  // Left
        20, 21, 22, 20, 22, 23,  // Right
    };
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    
    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Cube::~Cube() {
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Cube::draw(const glm::mat4& viewProjMtx, GLuint shader,bool floor) {
    if (isInvisible) return;

    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    // glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    glm::vec3 color_floor = glm::vec3(0.40f, 0.20f, 0.10f);

    if (floor){
        glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color_floor[0]);
    }else{
        glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    }

    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube::update() {
    // Spin the cube
    // spin(0.05f);
    #ifdef __APPLE__
        gravity = -0.00035f;
        initialJumpVelocity = 0.030f;
    #endif
    // if (isJumping) {
    //     //update jump height
    //     jumpVelocity += gravity; 
    //     jumpHeight += jumpVelocity;

    //     if (jumpHeight <= 0.0f) {
    //         jumpHeight = 0.0f;
    //         isJumping = false;
    //         jumpVelocity = 0.0f;
    //         isGrounded = true;
    //     }

    //     //glm::mat4 baseModel = glm::translate(glm::mat4(1.0f), glm::vec3(0, jumpHeight, 0));
    //     // model = glm::translate(glm::mat4(1.0f), glm::vec3(0, jumpHeight, 0)) * baseModel;
    // }
    model = glm::translate(baseModel, glm::vec3(0.0f, jumpHeight, 0.0f));
    
    if (isCarryingArtifact && carriedArtifact != nullptr) 
    {
        glm::vec3 offset(1.0f, 1.0f, 0.0f);
        glm::mat4 newBaseModel = glm::translate(baseModel, offset);
        carriedArtifact->setBaseModel(newBaseModel);
    }
}

void Cube::userInput(int key){
    switch(key) {
        /*case GLFW_KEY_W:
            model = glm::translate(model, glm::vec3(0,0,-0.5f));
            break;
        case GLFW_KEY_A:
            model = glm::translate(model, glm::vec3(-0.5f,0,0));
            break;
        case GLFW_KEY_S:
            model = glm::translate(model, glm::vec3(0,0,0.5f));
            break;
        case GLFW_KEY_D:
            model = glm::translate(model, glm::vec3(0.5f,0,0));
            break;
        case GLFW_KEY_T:
            model = glm::translate(model, glm::vec3(0,0.5f,0));
            break;
        case GLFW_KEY_G:
            model = glm::translate(model, glm::vec3(0,-0.5f,0));
            break;
        case GLFW_KEY_K:
            model = glm::rotate(model, 0.1f, glm::vec3(0,1,0));
            break;
        case GLFW_KEY_L:
            model = glm::rotate(model, -0.1f, glm::vec3(0,1,0));
            break;*/
        case GLFW_KEY_SPACE:
        if (isGrounded) {
            isJumping = true;
            isGrounded = false;   
            jumpVelocity = initialJumpVelocity;
        }
        case GLFW_KEY_F:
            attemptGrabArtifact();
            break;

        default:
            break;
    }


}
void Cube::handleContinuousInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir) {
    glm::vec3 movement(0.0f);
    float speed = 0.02f;

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
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        baseModel = glm::translate(baseModel, glm::vec3(0, 0.005f, 0));

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        baseModel = glm::translate(baseModel, glm::vec3(0, -0.005f, 0));

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        baseModel = glm::rotate(baseModel, 0.02f, glm::vec3(0, 1, 0));

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        baseModel = glm::rotate(baseModel, -0.02f, glm::vec3(0, 1, 0));

    /*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping) {
        isJumping = true;
        jumpVelocity = initialJumpVelocity;
    }*/
   if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (!eWasPressed) {
            switch (type) {
                case CHARACTER_1: {
                    // Invisibility
                    if (!isInvisible) {
                        isInvisible = true;
                        invisibleStartTime = glfwGetTime();
                    }
                    break;
                }
                case CHARACTER_2: {
                    if (!isSpeedBoosted) {
                        isSpeedBoosted = true;
                        speedBoostStartTime = glfwGetTime();
                        speed = boostedSpeed;
                    }
                    break;
                }
                case CHARACTER_3: {
                    isAltColor = !isAltColor;
                    break;
                }
                case CHARACTER_4: {
                    if (!radarUsed) {
                        radarUsed = true; 
                    }
                    break;
                }
            }
        }
        eWasPressed = true;
    } else {
        eWasPressed = false;
    }
}

void Cube::updateFromPacket(const InitPlayerPacket& packet) {
    memcpy(&model, packet.model, sizeof(packet.model));
    isInvisible = packet.isInvisible;
    // printState();
}

void Cube::spin(float deg) {
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Cube::getPosition() const {
    return glm::vec3(model[3]);  // extract translation from matrix
}

void Cube::setColor(const glm::vec3& change_color) {
    color = change_color;
}

void Cube::setBaseModel(const glm::mat4& change_model) {
    model = change_model;
}

void Cube::attemptGrabArtifact() {
    if (carriedArtifact == nullptr) 
    {
        std::cout << "carriedArtifact is NULL!" << std::endl;
        return;
    }

    if (!isCarryingArtifact && carriedArtifact != nullptr) {
        glm::vec3 cubePos = getPosition();
        glm::vec3 artifactPos = carriedArtifact->getPosition();
        float distance = glm::length(cubePos - artifactPos);
        std::cout << "Distance = " << distance << std::endl;

        if (distance < 1.5f) {
            isCarryingArtifact = true;
            std::cout << "Picked up!" << std::endl;
        }
    }
}
void Cube::setCarriedArtifact(Cube* artifact) {
    carriedArtifact = artifact;
}

void Cube::setBaseAndModel(const glm::mat4& mtx) {
    baseModel = mtx;
    // model = glm::translate(baseModel, glm::vec3(0.0f, jumpHeight, 0.0f));
    model = baseModel;
}

bool Cube::didUseRadarAbility() {
    if (radarUsed) {
        radarUsed = false; 
        return true;
    }
    return false;
}