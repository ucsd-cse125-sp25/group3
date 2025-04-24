////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

Camera::Camera() {
    Reset();
}
void Camera::Update(const glm::vec3& targetPos) {
    // Step 1: Start with identity matrix
    glm::mat4 world(1.0f);

    // Step 2: Move camera back by Distance (along local z)
    world[3][2] = Distance;

    // Step 3: Rotate the camera around target
    world = glm::eulerAngleY(glm::radians(-Azimuth)) *
            glm::eulerAngleX(glm::radians(-Incline)) *
            world;

    // Step 4: Move to cube's position
    world = glm::translate(glm::mat4(1.0f), targetPos) * world;

    // Step 5: Compute view matrix
    glm::mat4 view = glm::inverse(world);

    // Step 6: Perspective projection
    glm::mat4 project = glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

    // Step 7: Final view-projection matrix
    ViewProjectMtx = project * view;

    Eye = glm::vec3(world[3]);  // camera position
    Center = targetPos;         // cube position
}
void Camera::Reset() {
    FOV = 45.0f;
    Aspect = 1.33f;
    NearClip = 0.1f;
    FarClip = 100.0f;

    Distance = 10.0f;
    Azimuth = 0.0f;
    Incline = 20.0f;
}

glm::vec3 Camera::GetForwardVector() const {
    return glm::normalize(Center - Eye);  
}

