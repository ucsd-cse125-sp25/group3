#include "Scene.h"
#include <iostream>
#include <string>
#include "glm/gtx/string_cast.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Scene::Scene(){
}

Scene::~Scene(){
    for (AnInstance* i : instances){
        delete i;
    }
}

void Scene::addInstance(AnInstance* inst){
    if (inst != nullptr){
        instances.push_back(inst);
    }
}

void Scene::addInstances(std::vector<AnInstance*>& insts){
    for (AnInstance* i : insts){
        addInstance(i);
    }
}

std::vector<AnInstance*>* Scene::getInstances(){
    return &instances;
}

void Scene::draw(const glm::mat4& viewProjMtx, ShaderManager* shaderManager){
    for (int i = 0; i < instances.size(); i++){
        instances[i]->draw(viewProjMtx, shaderManager);
    }
}

void Scene::update(AnimationPlayer* animPlayer){
    for (int i = 0; i < instances.size(); i++){
        instances[i]->update(animPlayer);
    }
}