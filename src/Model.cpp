#include "AnimationPlayer.h"
#include "Model.h"
#include <iostream>
#include <string>
#include "glm/gtx/string_cast.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*
ONLY USE THIS IF THERE IS NO SKELETON FOR THIS MODEL!
*/

Model::Model(ModelType mType, const std::string & file, TextureManager* textureManager){
    model = glm::mat4(1.0f);
    
    modelType = mType;

    load(file, textureManager);

    this->skel = nullptr;

    setupBuf();
}

Model::Model(ModelType mType, AnimationPlayer* animPlayer, const std::string & file, TextureManager* textureManager){
    model = glm::mat4(1.0f);
    
    modelType = mType;

    load(file, textureManager);

    // sets the skeleton of the model, if there is no matching skeleton, it is set to null
    setSkel(animPlayer);

    setupBuf();
}

Model::~Model(){
    for (MeshInstance* mi : meshInstances){
        delete mi;
    }

    for (Mesh* m : meshes){
        delete m;
    }
}

ModelType Model::getModelType() {
    return modelType;
}

void Model::recLoad(const aiNode* node, glm::mat4 parent){
    glm::mat4 currMat = parent * aiMatToGLM(node->mTransformation);

    for (int i = 0; i < node->mNumMeshes; i++){
        MeshInstance* mi = new MeshInstance(meshes[node->mMeshes[i]], currMat);
        meshInstances.push_back(mi);
    }

    for (int i = 0; i < node->mNumChildren; i++){
        recLoad(node->mChildren[i], currMat);
    }
}

void Model::process(const aiScene* scene, TextureManager* textureManager) {
    if (scene->HasMeshes()){
        
        meshes.reserve(scene->mNumMeshes);
        for (int i = 0; i < scene->mNumMeshes; i++){
            Mesh* m = new Mesh();
            if (m->setMesh(textureManager, scene->mMeshes[i], scene)){
                meshes.push_back(m);
            };
        }

        recLoad(scene->mRootNode, glm::mat4(1.0f));
    }
}

bool Model::load(const std::string& file, TextureManager* textureManager){
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file, 
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_GenNormals);

    if (nullptr == scene) {
        std::cerr << "Failed to load model" << file << std::endl;
        return false;
    }

    process(scene, textureManager);

    return true;
}

void Model::setModelType(ModelType mType){
    modelType = mType;
}

void Model::setMMat(glm::mat4 mMat){
    model = mMat;
}

void Model::addMesh(Mesh* mesh){
    if (mesh != nullptr){
        meshes.push_back(mesh);
    }
}

void Model::setSkel(AnimationPlayer* animPlayer){
    Skeleton* sk = animPlayer->getSkel(this->modelType);
    // if (this->modelType == ModelType::SecurityGuard){
    //     std::cout << "securityguard" << std::endl;
    // }
    if (sk != nullptr){
        this->skel = sk;
        for (int i = 0; i < meshes.size(); i++){
            meshes[i]->setSkel(sk);
        }   
    } else {
        std::cout << "no matching skeleton found" << std::endl;
    }
}

int Model::getNumMeshes(){
    return meshes.size();
}

int Model::getNumMeshInstances(){
    return meshInstances.size();
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

void Model::setupBuf(){
    for (int i = 0; i < meshes.size(); i++){
        meshes[i]->setupBuf();
    }
}

void Model::draw(std::vector<std::vector<glm::mat4>>& jointMats, const glm::mat4& viewProjMtx, ShaderManager* shaderManager){
    glm::mat4 VPMmMtx = viewProjMtx * model;
    assert(jointMats.size() == meshInstances.size());
    for (int i = 0; i < meshInstances.size(); i++){
        assert(jointMats[i].size() == MAX_JOINTS);
        // meshes[i]->setMMat(model);
        // std::cout<< "i: " << i << std::endl;
        meshInstances[i]->draw(jointMats[i], VPMmMtx, shaderManager);
    }
}

void Model::update(std::vector<std::vector<glm::mat4>>& jointMats){
    for (int i = 0; i < meshInstances.size(); i++){
        meshInstances[i]->update(jointMats[i]);
    }
}

// void Model::handleInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir) {
//     glm::vec3 movement(0.0f);
//     float speed = 0.02f;

//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//         movement += forwardDir;
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         movement -= forwardDir;
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//         movement += rightDir;
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//         movement -= rightDir;

//     if (glm::length(movement) > 0.0f) {
//         movement = glm::normalize(movement) * speed;
//         model = glm::translate(model, movement);
//     }

//     if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
//         model = glm::translate(model, glm::vec3(0, 0.005f, 0));
//     if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
//         model = glm::translate(model, glm::vec3(0, -0.005f, 0));
//     if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
//         model = glm::rotate(model, 0.02f, glm::vec3(0, 1, 0));
//     if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
//         model = glm::rotate(model, -0.02f, glm::vec3(0, 1, 0));
// }
