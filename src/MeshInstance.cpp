#include "MeshInstance.h"

MeshInstance::MeshInstance() {}

MeshInstance::MeshInstance(Mesh* mesh, glm::mat4 mat) {
    this->mesh = mesh;
    this->model = mat;
}

MeshInstance::~MeshInstance() {}

void MeshInstance::setMesh(Mesh* mesh){
    this->mesh = mesh;
}

void MeshInstance::setMMat(glm::mat4 model) {
    this->model = model;
}

void MeshInstance::draw(std::vector<glm::mat4>& mMat, const glm::mat4& viewProjMtx, ShaderManager* shaderManager) {
    mesh->draw(model, mMat, viewProjMtx, shaderManager);
}

void MeshInstance::update(std::vector<glm::mat4>& mMat) {
    mesh->update(mMat);
}