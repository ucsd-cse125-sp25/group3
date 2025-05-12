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

//viewProjMatrix, eye, center, azimuth, incline, aspect
void Camera::toVector(std::vector<char>* vec) {
    char buf[4];

    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            // printf("elem i=%d, j=%d: %f\n", i,j,baseModel[i][j]);
            memcpy(buf, &ViewProjectMtx[i][j], sizeof(float));
            vec->insert(vec->end(), &buf[0], &buf[4]);
        }
    }

    for (int i=0; i<3; i++) {
        memcpy(buf, &Eye[i], sizeof(float));
        vec->insert(vec->end(), &buf[0], &buf[4]);
    }

    for (int i=0; i<3; i++) {
        memcpy(buf, &Center[i], sizeof(float));
        vec->insert(vec->end(), &buf[0], &buf[4]);
    }
    memcpy(buf, &Azimuth, sizeof(float));
    vec->insert(vec->end(), &buf[0], &buf[4]);
    memcpy(buf, &Incline, sizeof(float));
    vec->insert(vec->end(), &buf[0], &buf[4]);
    memcpy(buf, &Aspect, sizeof(float));
    vec->insert(vec->end(), &buf[0], &buf[4]);
}

int Camera::readFromArray(char * data) {
    int totalBytes = 0;

    for (int i=0; i<(16+3+3+3); i++) {
        memcpy(&updatedVals[i*4], &data[i*4], sizeof(float));
        totalBytes += sizeof(float);
    }
    // for (int i=0; i<16; i++) {
    //     memcpy(&ViewProjectMtx[i/4][i%4], &data[i*4], sizeof(float));
    //     totalBytes += sizeof(float);
    // }

    // for (int i=0; i<3; i++) {
    //     memcpy(&Eye[i], &data[sizeof(ViewProjectMtx) + i*4], sizeof(float));
    //     totalBytes += sizeof(float);
    // }

    // for (int i=0; i<3; i++) {
    //     memcpy(&Center[i], &data[sizeof(ViewProjectMtx) + sizeof(Eye) + i*4], sizeof(float));
    //     totalBytes += sizeof(float);
    // }
    return totalBytes;
}

void Camera::applyUpdates() {
    int offset = 0;

    for (int i=0; i<16; i++) {
        memcpy(&ViewProjectMtx[i/4][i%4], &updatedVals[i*4], sizeof(float));
        // totalBytes += sizeof(float);
    }
    offset += sizeof(ViewProjectMtx);

    for (int i=0; i<3; i++) {
        memcpy(&Eye[i], &updatedVals[offset + i*4], sizeof(float));
        // totalBytes += sizeof(float);
    }
    offset += sizeof(Eye);

    for (int i=0; i<3; i++) {
        memcpy(&Center[i], &updatedVals[offset + i*4], sizeof(float));
        // totalBytes += sizeof(float);
    }
    offset += sizeof(Center);
    memcpy(&Azimuth, &updatedVals[offset], sizeof(float));
    offset += sizeof(Azimuth);
    memcpy(&Incline, &updatedVals[offset], sizeof(float));
    offset += sizeof(Incline);
    memcpy(&Aspect, &updatedVals[offset], sizeof(float));
}

