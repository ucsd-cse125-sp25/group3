#include "core.h"
#include <assimp/scene.h>
#include <glm/gtx/quaternion.hpp>


glm::vec3 aiColToGLM(const aiColor3D& v);

glm::vec3 aiVecToGLM(const aiVector3D& v);

glm::quat aiQuatToGLM(const aiQuaternion& q);

glm::mat4 aiMatToGLM(const aiMatrix4x4& m);