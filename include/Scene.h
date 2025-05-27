#pragma once

#include <vector>
#include "core.h"
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Scene {
private:
    std::vector<Model*> models;

    void process(const aiScene* scene);
    
    const aiScene* rawScene = nullptr;
    Assimp::Importer importer; 

public:
    Scene();
    ~Scene();

    bool load(const std::string & file);
    void addModel(Model* model);

    std::vector<Model*>* getModels();
    Model* getModel(int i);
    void removeModel(int i);

    void setupBufAll();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();

    const aiScene* getAiScene() const { return rawScene; }
};