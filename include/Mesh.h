#pragma once

#include <vector>
#include "core.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <map>
#include "utils.h"
#include "Skeleton.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#define MAX_JOINT_INFLUENCE 8
#define MAX_JOINT_INFLUENCE_GPU 4 
#define MAX_JOINTS 100

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    int jointIDs1[MAX_JOINT_INFLUENCE_GPU];
    float weights1[MAX_JOINT_INFLUENCE_GPU];
    int jointIDs2[MAX_JOINT_INFLUENCE_GPU];
    float weights2[MAX_JOINT_INFLUENCE_GPU];
    int numJoints;
};

struct invBMatInfo{
    int id;
    glm::mat4 invBMat;
};

class Mesh {
private:

    GLuint VAO;
    GLuint VBO_pn, VBO_uv, EBO;

    GLuint tex;

    enum RenderMode renderMode;

    Skeleton* skel;
    // glm::mat4 model;
    glm::vec3 color;

    std::vector<Vertex> vertices;
    // std::vector<Vertex> verticesRaw;

    std::vector<glm::vec2> uvs;

    //which vertices correspond to triangle
    std::vector<unsigned int> indices;

    std::map<std::string, invBMatInfo> invBMats;

    // std::vector<glm::mat4> mMat;

    int numJoints;

    void setDefaultJointVal(Vertex &v);

public:
    Mesh();
    Mesh(glm::vec3 color);
    ~Mesh();

    /*
    Sets the UV Texture corresponding to the Mesh.
    */
    void setTex(GLuint texID);

    void setJointVals(const aiMesh* mesh);

    void setColor(glm::vec3 color);

    bool setMesh(TextureManager* textureManager, const aiMesh* mesh, const aiScene* scene);

    void setMaterials(TextureManager* textureManager, const aiMesh* mesh, const aiScene *scene);

    void setSkel(Skeleton* skel);

    // void setMMat(glm::mat4 model);
    
    void setupBuf();

    void draw(glm::mat4 model, std::vector<glm::mat4>& mMat, const glm::mat4& viewProjMtx, ShaderManager* shaderManager);
    void update(std::vector<glm::mat4>& mMat);
    
};