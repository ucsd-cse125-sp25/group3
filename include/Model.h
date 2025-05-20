#pragma once

#include <vector>
#include "core.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <map>
#include "CharState.h"

/*
The Model class should handle everything about a 3D model.

It can be used to move and play animations on a collection of meshes
*/
class Model {
private:
    std::vector<Mesh*> meshes;

    glm::mat4 model;

    enum CharState state;

    void process(const aiScene* scene);

public:

    /*
    Creates the model and sets the state to IDLE.

    Once created, meshes should be added to the model

    Call update(), then call setupBuf() once at the beginning.
    */
    Model();

    /*
    Deletes all meshes controlled by the Model.
    */
    ~Model();

    /*
    All meshes in the scene in the file are loaded in and placed in the model

    @param file A file containing a 3D model.
    */
    bool load(const std::string & file);

    /*
    Sets 'model', the model matrix, of the Model.

    Through the matrix, the position, scale, and rotation of the 
    model (all meshes stored in the model) will be changed together. 
    */
    void setMMat(glm::mat4 mMat);

    /*
    mesh will now be moved by the model matrix of the Model.

    The mesh will also be deleted when the model is deleted.
    */
    void addMesh(Mesh* mesh);

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

    void setSkel(Skeleton* skel);

    /*
    Sets up the buffers for OpenGL, must be called before draw().
    */
    void setupBuf();

    /*
    Draws the meshes connected to this model using the shader in the current state.
    */
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    
    /*
    Updates matrices of meshes in model to match animation.
    */
    void update();

    void handleInput(GLFWwindow* window, const glm::vec3& forwardDir, const glm::vec3& rightDir);

};