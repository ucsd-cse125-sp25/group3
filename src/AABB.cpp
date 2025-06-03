#include "AABB.h"
#include <iostream>
#include <limits>
#include <functional>

AABB::AABB() {
    min = glm::vec3(std::numeric_limits<float>::max());
    max = glm::vec3(std::numeric_limits<float>::lowest());
}

AABB::AABB(const glm::vec3& minB, const glm::vec3& maxB)
    : min(minB), max(maxB) {}

bool AABB::contains(const glm::vec3& point) const {
    return (point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y &&
            point.z >= min.z && point.z <= max.z);
}

bool AABB::intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x &&
            min.y <= other.max.y && max.y >= other.min.y &&
            min.z <= other.max.z && max.z >= other.min.z);
}

void AABB::expandToInclude(const glm::vec3& point) {
    min = glm::min(min, point);
    max = glm::max(max, point);
}

glm::vec3 AABB::getCenter() const {
    return 0.5f * (min + max);
}

glm::vec3 AABB::getSize() const {
    return max - min;
}

void AABB::print() const {
    std::cout << "AABB Min: (" << min.x << ", " << min.y << ", " << min.z << ")\n";
    std::cout << "AABB Max: (" << max.x << ", " << max.y << ", " << max.z << ")\n";
    std::cout << "Size: (" << getSize().x << ", " << getSize().y << ", " << getSize().z << ")\n";
}

AABB AABB::fromScene(const aiScene* scene) {
    AABB box;

    std::function<void(aiNode*, aiMatrix4x4)> processNode = [&](aiNode* node, aiMatrix4x4 transform) {
        aiMatrix4x4 currentTransform = transform * node->mTransformation;
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                aiVector3D v = mesh->mVertices[j];
                v *= currentTransform;
                box.expandToInclude(glm::vec3(v.x, v.y, v.z));
            }
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], currentTransform);
        }
    };

    processNode(scene->mRootNode, aiMatrix4x4());
    return box;
}

std::unordered_map<std::string, AABB> AABB::fromSceneMeshes(const aiScene* scene) {
    std::unordered_map<std::string, AABB> result;

    std::function<void(aiNode*, aiMatrix4x4)> processNode = [&](aiNode* node, aiMatrix4x4 transform) {
        aiMatrix4x4 currentTransform = transform * node->mTransformation;
        AABB box;

        bool hasMesh = false;
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                aiVector3D v = mesh->mVertices[j];
                v *= currentTransform;
                box.expandToInclude(glm::vec3(v.x, v.y, v.z));
                hasMesh = true;
            }
        }

        if (hasMesh) {
            std::string name = node->mName.C_Str();
            if (name.empty()) {
                static int unnamedCount = 0;
                name = "unnamed_" + std::to_string(unnamedCount++);
            }
            result[name] = box;
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], currentTransform);
        }
    };

    processNode(scene->mRootNode, aiMatrix4x4());
    return result;
}

void AABB::draw(const glm::mat4& viewProjMtx, GLuint shader) const {
    static GLuint VAO = 0, VBO = 0, EBO = 0;
    static bool initialized = false;

    glm::vec3 corners[8] = {
        {min.x, min.y, min.z},
        {max.x, min.y, min.z},
        {max.x, max.y, min.z},
        {min.x, max.y, min.z},
        {min.x, min.y, max.z},
        {max.x, min.y, max.z},
        {max.x, max.y, max.z},
        {min.x, max.y, max.z},
    };

    GLuint indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0,  // bottom
        4, 5, 5, 6, 6, 7, 7, 4,  // top
        0, 4, 1, 5, 2, 6, 3, 7   // sides
    };

    if (!initialized) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        initialized = true;

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(corners), corners);

    glm::mat4 model = glm::mat4(1.0f);  

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, &viewProjMtx[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, &model[0][0]);

    glm::vec3 debugColor = glm::vec3(1.0f, 0.0f, 0.0f);  // 红色 AABB
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &debugColor[0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); 
    glBindVertexArray(0);

    glUseProgram(0);
}