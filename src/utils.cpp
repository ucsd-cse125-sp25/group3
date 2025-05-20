#include "utils.h"

glm::vec3 aiVecToGLM(const aiVector3D& v){
    return glm::vec3(v.x, v.y, v.z);
}

glm::quat aiQuatToGLM(const aiQuaternion& q){
    return glm::quat(q.w, q.x, q.y, q.z);
}

glm::mat4 aiMatToGLM(const aiMatrix4x4& m){
    glm::mat4 gM;

    gM[0][0] = m.a1; gM[1][0] = m.a2; gM[2][0] = m.a3; gM[3][0] = m.a4;
    gM[0][1] = m.b1; gM[1][1] = m.b2; gM[2][1] = m.b3; gM[3][1] = m.b4;
    gM[0][2] = m.c1; gM[1][2] = m.c2; gM[2][2] = m.c3; gM[3][2] = m.c4;
    gM[0][3] = m.d1; gM[1][3] = m.d2; gM[2][3] = m.d3; gM[3][3] = m.d4;
    
    return gM;
}