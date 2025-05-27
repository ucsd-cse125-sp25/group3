#pragma once

#include <vector>
#include "core.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <map>
#include "ModelType.h"
#include "ShaderManager.h"

/*
The Model class should handle everything about a 3D model.

It can be used to move and play animations on a collection of meshes
*/
class Model {
private:
    ModelType modelType;

    std::vector<Mesh*> meshes;

    Skeleton* skel;

    void process(const aiScene* scene);

public:

    /*
    Creates the model

    Once created, meshes should be added to the model

    Call update(), then call setupBuf() once at the beginning.
    */
    Model(ModelType mType, const std::string & file);

    /*
    Deletes all meshes controlled by the Model.
    */
    ~Model();

    ModelType getModelType();

    /*
    All meshes in the scene in the file are loaded in and placed in the model

    @param file A file containing a 3D model.
    */
    bool load(const std::string & file);

    void setModelType(ModelType mType);

    // /*
    // Sets 'model', the model matrix, of the Model.

    // Through the matrix, the position, scale, and rotation of the 
    // model (all meshes stored in the model) will be changed together. 
    // */
    // void setMMat(glm::mat4 mMat);

    /*
    mesh will now be moved by the model matrix of the Model.

    The mesh will also be deleted when the model is deleted.
    */
    void addMesh(Mesh* mesh);

    /*
    Gets all meshes controlled by this model as a list
    */
    int getNumMeshes();

    /*
    Gets all meshes controlled by this model as a list
    */
    std::vector<Mesh*>* getMeshes();

    /*
    Gets a mesh located at index i in the list of meshes controlled by this model
    */
    Mesh* getMesh(int i);

    /*
    Removes (but does not delete) the mesh located at index i in the list of meshes controlled by this model

    Once removed, the mesh will no longer be deleted when the model is deleted.
    */
    void removeMesh(int i);

    void setSkel(AnimationPlayer* animPlayer);

    /*
    Sets up the buffers for OpenGL, must be called before draw().
    */
    void setupBuf();

    /*
    Draws the meshes connected to this model using the shader in the current state.
    */

    void draw(const glm::mat4& model, std::vector<std::vector<glm::mat4>>& jointMats, const glm::mat4& viewProjMtx, ShaderManager* shaderManager);
    
    /*
    Updates matrices of meshes in model to match animation.
    */
    void update(std::vector<std::vector<glm::mat4>>& jointMats);

    // void handleInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir);

};