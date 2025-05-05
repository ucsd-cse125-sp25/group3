#include "Model.h"
#include <iostream>
#include <string>
#include "glm/gtx/string_cast.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Model::Model(){
    model = glm::mat4(1.0f);
}

Model::~Model(){
    for (Mesh* m : meshes){
        delete m;
    }
}

void Model::process(const aiScene* scene) {
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

bool Model::load(const std::string& file){
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

void Model::setMMat(glm::mat4 mMat){
    model = mMat;
}

void Model::addMesh(Mesh* mesh){
    if (mesh != nullptr){
        meshes.push_back(mesh);
    }
}


std::vector<Mesh*>* Model::getMeshes() {
    return &meshes;
}

Mesh* Model::getMesh(int i){
    if (0 <= i && i < meshes.size()){
        return meshes[i];
    }
    return nullptr;
}

void Model::deleteMesh(int i){
    delete meshes[i];
    meshes.erase(meshes.begin() + i);
}

void Model::setupBuf(){
    for (int i = 0; i < meshes.size(); i++){
        meshes[i]->setupBuf();
    }
}

void Model::draw(const glm::mat4& viewProjMtx, GLuint shader){
    glm::mat4 VPMmMtx = viewProjMtx * model;
    for (int i = 0; i < meshes.size(); i++){
        meshes[i]->draw(VPMmMtx, shader);
    }
}

void Model::update(){
    for (int i = 0; i < meshes.size(); i++){
        meshes[i]->update();
    }
}