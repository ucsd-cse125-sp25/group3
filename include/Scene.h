#pragma once

#include <vector>
#include "core.h"
#include "AnInstance.h"
#include "ShaderManager.h"
#include "AnimationPlayer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Scene {
private:
    std::vector<AnInstance*> instances;

public:
    Scene();
    ~Scene();

    void addInstance(AnInstance* inst);

    void addInstances(std::vector<AnInstance*>& insts);

    std::vector<AnInstance*>* getInstances();

    // void setupBufAll();

    void draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager);
    void update(AnimationPlayer* animPlayer);
};