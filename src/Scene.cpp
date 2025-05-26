#include "Scene.h"
#include "Mesh.h"
#include <iostream>
#include <string>
#include "glm/gtx/string_cast.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Scene::Scene(){
}

Scene::~Scene(){
    for (Model* m : models){
        delete m;
    }
}

bool Scene::process(const aiScene* scene) {
    if (scene->HasMeshes()){
        models.reserve(scene->mNumMeshes);
        for (int i = 0; i < scene->mNumMeshes; i++){
            Mesh* mesh = new Mesh();
            if (mesh->setMesh(scene->mMeshes[i], scene)){
                Model* model = new Model();
                model->addMesh(mesh);
                models.push_back(model);
            } else {
                return false;
            }
        }
    }

    return true;
}

bool Scene::load(const std::string& file){
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file, 
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_GenNormals);

    if (nullptr == scene) {
        return false;
    }

    return process(scene);
}

void Scene::addModel(Model* model){
    if (model != nullptr){
        models.push_back(model);
    }
}

std::vector<Model*>* Scene::getModels(){
    return &models;
}

Model* Scene::getModel(int i){
    if (0 <= i && i < models.size()){
        return models[i];
    }
    return nullptr;
}

void Scene::setupBufAll(){
    for (int i = 0; i < models.size(); i++){
        models[i]->setupBuf();
    }
}

void Scene::draw(const glm::mat4& viewProjMtx, GLuint shader){
    for (int i = 0; i < models.size(); i++){
        models[i]->draw(viewProjMtx, shader);
    }
}

void Scene::update(){
    for (int i = 0; i < models.size(); i++){
        models[i]->update();
    }
}