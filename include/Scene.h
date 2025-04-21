#pragma once

#include <vector>
#include "core.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Scene {
private:
    std::vector<Mesh*> meshes;

    void process(const aiScene* scene);

public:
    Scene();
    ~Scene();

    bool load(const std::string & file);
    void addMesh(Mesh* mesh);

    Mesh* getMesh(int i);

    void setupBufAll();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();
};