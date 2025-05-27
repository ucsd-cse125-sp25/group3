#pragma once

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class AABB {
public:

    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    glm::vec3 min;
    glm::vec3 max;

    AABB(); 
    void expandToInclude(const glm::vec3& point);

    glm::vec3 getCenter() const;
    glm::vec3 getSize() const;

    void print() const;
    void draw(const glm::mat4& VP, GLuint shaderProgram) const;

    static AABB fromScene(const aiScene* scene);

    static std::unordered_map<std::string, AABB> fromSceneMeshes(const aiScene* scene);
};
