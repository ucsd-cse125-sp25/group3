////////////////////////////////////////
// Camera.h
////////////////////////////////////////

#pragma once

#include "core.h"
#include "glm/gtx/euler_angles.hpp"
#include <vector>

// The Camera class provides a simple means to controlling the 3D camera. It could
// be extended to support more interactive controls. Ultimately. the camera sets the
// GL projection and viewing matrices.

class Camera {
public:
    Camera();

    // void Update();
    void Update(const glm::vec3& targetPos);
    void Reset();

    // Access functions
    void SetAspect(float a) { Aspect = a; }
    void SetDistance(float d) { Distance = d; }
    void SetAzimuth(float a) { Azimuth = a; }
    void SetIncline(float i) { Incline = i; }

    float GetDistance() { return Distance; }
    float GetAzimuth() { return Azimuth; }
    float GetIncline() { return Incline; }

    void SetOrtho(float left, float right, float bottom, float top, float near, float far);
    void SetLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

    const glm::mat4 &GetViewProjectMtx() { return ViewProjectMtx; }

    glm::vec3 GetForwardVector() const;

    void toVector(std::vector<char> * vec);
    int readFromArray(char * data);

private:
    // Perspective controls
    float FOV;       // Field of View Angle (degrees)
    float Aspect;    // Aspect Ratio
    float NearClip;  // Near clipping plane distance
    float FarClip;   // Far clipping plane distance

    // Polar controls
    float Distance;  // Distance of the camera eye position to the origin (meters)
    float Azimuth;   // Rotation of the camera eye position around the Y axis (degrees)
    float Incline;   // Angle of the camera eye position over the XZ plane (degrees)

    // Computed data
    glm::mat4 ViewProjectMtx;

    glm::vec3 Eye;    
    glm::vec3 Center; 

    glm::mat4 projection;
    bool isOrtho = false;
    void updateProjection();
};