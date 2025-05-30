#pragma once

#include <vector>
#include "core.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <map>
#include "ModelType.h"
#include "ShaderManager.h"
#include "AnimationPlayer.h"
#include "TextureManager.h"
#include "MeshInstance.h"

/*
The Model class should handle everything about a 3D model.

It can be used to move and play animations on a collection of meshes
*/
class Model {
private:
    ModelType modelType;

    std::vector<Mesh*> meshes;

    std::vector<MeshInstance*> meshInstances;

    glm::mat4 model;

    Skeleton* skel;

    void process(const aiScene* scene, TextureManager* textureManager);

    void recLoad(const aiNode* node, glm::mat4 parent);

    /*
    mesh will now be moved by the model matrix of the Model.

    The mesh will also be deleted when the model is deleted.
    */
    void addMesh(Mesh* mesh);

public:

    /*
    Creates the model

    Once created, meshes should be added to the model

    Call update(), then call setupBuf() once at the beginning.
    */
    Model(ModelType mType, const std::string & file, TextureManager* textureManager);

    /*
    Deletes all meshes controlled by the Model.
    */
    ~Model();

    ModelType getModelType();

    /*
    All meshes in the scene in the file are loaded in and placed in the model

    @param file A file containing a 3D model.
    */
    bool load(const std::string & file, TextureManager* textureManager);

    void setModelType(ModelType mType);

    /*
    Sets 'model', the model matrix, of the Model.

    Through the matrix, the position, scale, and rotation of the 
    model (all meshes stored in the model) will be changed together. 
    */
    void setMMat(glm::mat4 mMat);

    /*
    Gets all meshes controlled by this model as a list
    */
    int getNumMeshes();

    /*
    Gets all mesh instances controlled by this model as a list
    */
    int getNumMeshInstances();

    /*
    Gets all meshes controlled by this model as a list
    */
    std::vector<Mesh*>* getMeshes();

    /*
    Gets a mesh located at index i in the list of meshes controlled by this model
    */
    Mesh* getMesh(int i);

    void setSkel(AnimationPlayer* animPlayer);

    /*
    Sets up the buffers for OpenGL, must be called before draw().
    */
    void setupBuf();

    /*
    Draws the meshes connected to this model using the shader in the current state.
    */

    void draw(std::vector<std::vector<glm::mat4>>& jointMats, const glm::mat4& viewProjMtx, ShaderManager* shaderManager);
    
    /*
    Updates matrices of meshes in model to match animation.
    */
    void update(std::vector<std::vector<glm::mat4>>& jointMats);

    // void handleInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir);

};