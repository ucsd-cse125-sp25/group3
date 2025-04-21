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
    for (Mesh* m : meshes){
        delete m;
    }
}

void Scene::process(const aiScene* scene) {
    if (scene->HasMeshes()){
        meshes.reserve(scene->mNumMeshes);
        for (int i = 0; i < scene->mNumMeshes; i++){
            Mesh* m = new Mesh();
            if (m->setMesh(scene->mMeshes[i])){
                meshes.push_back(m);
            };
        }
    }
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

    process(scene);

    return true;
}

void Scene::addMesh(Mesh* mesh){
    if (mesh != nullptr){
        meshes.push_back(mesh);
    }
}

Mesh* Scene::getMesh(int i){
    if (0 <= i && i < meshes.size()){
        return meshes[i];
    }
    return nullptr;
}

void Scene::setupBufAll(){
    for (int i = 0; i < meshes.size(); i++){
        meshes[i]->setupBuf();
    }
}

void Scene::draw(const glm::mat4& viewProjMtx, GLuint shader){
    for (int i = 0; i < meshes.size(); i++){
        meshes[i]->draw(viewProjMtx, shader);
    }
}

void Scene::update(){
    for (int i = 0; i < meshes.size(); i++){
        meshes[i]->update();
    }
}