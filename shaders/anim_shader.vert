#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 2) in ivec4 jointIDs;
layout (location = 3) in vec4 weights;

layout (location = 4) in vec2 vertexUV;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

const int MAX_JOINTS = 100;
const int MAX_JOINT_INFLUENCE = 4;
uniform mat4 finalJointMats[MAX_JOINTS];

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. 
out vec3 fragNormal;
out vec3 fragPos;
out vec2 uv;


void main()
{
    vec4 totalPos = vec4(0.0f);
    vec4 totalNorm = vec4(0.0f);
    
    for(int i = 0; i < MAX_JOINT_INFLUENCE; i++){
        if (jointIDs[i] == -1)
            continue;
        if (jointIDs[i] >= MAX_JOINTS)
        {
            totalPos = vec4(position, 1.0f);
            totalNorm = vec4(normal, 0.0f);
            break;
        }
        vec4 localPos = finalJointMats[jointIDs[i]] * vec4(position, 1.0f);
        //assumes no scale or skew
        vec4 localNorm = finalJointMats[jointIDs[i]]* vec4(normal, 0.0f);
        totalPos += weights[i] * localPos;
        totalNorm += weights[i] * localNorm;
    }
    totalNorm = normalize(totalNorm);
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = viewProj * model * totalPos;
    // gl_Position = viewProj * model * vec4(position, 1.0f);
    
    // for shading
	fragNormal = vec3(model * totalNorm);

    fragPos = vec3(model * totalPos);
    uv = vertexUV;

}