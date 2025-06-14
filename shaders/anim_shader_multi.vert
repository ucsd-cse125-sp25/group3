#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;

layout (location = 3) in ivec4 jointIDs1;
layout (location = 4) in vec4 weights1;
layout (location = 5) in ivec4 jointIDs2;
layout (location = 6) in vec4 weights2;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

const int MAX_JOINTS = 100;
const int MAX_JOINT_INFLUENCE = 8;
uniform mat4 finalJointMats[MAX_JOINTS];

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. 
out vec3 fragNormal;
out vec3 fragPos;

void main()
{
    vec4 totalPos = vec4(0.0f);
    vec4 totalNorm = vec4(0.0f);
    
    int exceed = 1;
    int skip = 0;
    for(int i = 0; i < 4; i++){
        if (jointIDs1[i] < 0){
            skip += 1;
            continue;
        }
        if (jointIDs1[i] >= MAX_JOINTS)
        {
            exceed = 0;
            totalPos = vec4(position, 1.0f);
            totalNorm = vec4(normal, 0.0f);
            break;
        }
        vec4 localPos = finalJointMats[jointIDs1[i]] * vec4(position, 1.0f);
        //assumes no scale or skew
        vec4 localNorm = finalJointMats[jointIDs1[i]]* vec4(normal, 0.0f);
        totalPos += weights1[i] * localPos;
        totalNorm += weights1[i] * localNorm;
    }
    
    if (exceed == 1){
        for(int i = 0; i < 4; i++){
            if (jointIDs2[i] < 0){
                skip += 1;
                continue;
            }
            if (jointIDs2[i] >= MAX_JOINTS)
            {
                totalPos = vec4(position, 1.0f);
                totalNorm = vec4(normal, 0.0f);
                break;
            }
            vec4 localPos = finalJointMats[jointIDs2[i]] * vec4(position, 1.0f);
            //assumes no scale or skew
            vec4 localNorm = finalJointMats[jointIDs2[i]]* vec4(normal, 0.0f);
            totalPos += weights2[i] * localPos;
            totalNorm += weights2[i] * localNorm;
        }
    }

    if (skip >= MAX_JOINT_INFLUENCE){
        totalPos = vec4(position, 1.0f);
        totalNorm = vec4(normal, 0.0f);
    }

    totalNorm = normalize(totalNorm);
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = viewProj * totalPos;
    
    // for shading
	fragNormal = vec3(model * totalNorm);

    fragPos = vec3(model * totalPos);
}