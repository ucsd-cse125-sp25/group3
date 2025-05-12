#pragma once

#include <vector>
#include "core.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model {
private:
    std::vector<Mesh*> meshes;

    glm::mat4 model;

    void process(const aiScene* scene);

public:
    Model();
    ~Model();

    bool load(const std::string & file);
    void setMMat(glm::mat4 mMat);
    void addMesh(Mesh* mesh);

    std::vector<Mesh*>* getMeshes();
    Mesh* getMesh(int i);
    void deleteMesh(int i);

    void setupBuf();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();

    void handleInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir);

};