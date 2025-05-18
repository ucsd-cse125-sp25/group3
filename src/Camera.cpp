////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

Camera::Camera() {
    Reset();
}
void Camera::Update(const glm::vec3& targetPos) {
    if (isOrtho) 
    {
        // Don't update orthographic camera this way
        ViewProjectMtx = projection * glm::lookAt(Eye, Center, glm::vec3(0, 1, 0));
        return;
    }
    
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

void Camera::updateProjection() 
{
    projection = glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);
}

void Camera::SetOrtho(float left, float right, float bottom, float top, float near, float far) 
{
    projection = glm::ortho(left, right, bottom, top, near, far);
    isOrtho = true;
}

void Camera::SetLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) 
{
    Eye = eye;
    Center = center;
    ViewProjectMtx = projection * glm::lookAt(eye, center, up);
}


void Camera::Reset() {
    FOV = 45.0f;
    Aspect = 1.33f;
    NearClip = 0.1f;
    FarClip = 100.0f;

    Distance = 10.0f;
    Azimuth = 0.0f;
    Incline = 20.0f;

    isOrtho = false;
    updateProjection();
}

glm::vec3 Camera::GetForwardVector() const {
    return glm::normalize(Center - Eye);  
}

void Camera::updateFromPacket(const StateUpdatePacket& packet, bool isMini) {
    if(!isMini) {
        memcpy(&ViewProjectMtx, packet.viewProjectMtx, sizeof(packet.viewProjectMtx));
        memcpy(&Eye, packet.eye, sizeof(packet.eye));
        memcpy(&Center, packet.center, sizeof(packet.center));
        Azimuth = packet.azimuth;
        Incline = packet.incline;
        Aspect = packet.aspect;
    }
    else {
        memcpy(&ViewProjectMtx, packet.miniViewProjectMtx, sizeof(packet.miniViewProjectMtx));
        memcpy(&Eye, packet.miniEye, sizeof(packet.miniEye));
        memcpy(&Center, packet.miniCenter, sizeof(packet.miniCenter));
        Azimuth = packet.miniAzimuth;
        Incline = packet.miniIncline;
        Aspect = packet.miniAspect;
    }
}