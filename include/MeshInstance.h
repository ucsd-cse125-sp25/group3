#pragma once

#include "core.h"
#include "Mesh.h"
#include <vector>
#include "ShaderManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "utils.h"

class MeshInstance {
    private:
        Mesh* mesh;

        glm::mat4 model;
    
    public:

    MeshInstance();

    MeshInstance(Mesh* mesh, glm::mat4 mat);

    ~MeshInstance();

    void setMesh(Mesh* mesh);

    void setMMat(glm::mat4 model);

    void draw(std::vector<glm::mat4>& mMat, const glm::mat4& viewProjMtx, ShaderManager* shaderManager);
    
    void update(std::vector<glm::mat4>& mMat);
    
};