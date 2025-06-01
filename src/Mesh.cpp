#include "Mesh.h"
#include <iostream>
#include <string>
#include "glm/gtx/string_cast.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "utils.h"


Mesh::Mesh(){
    skel = NULL;
    // model = glm::mat4(1.0f);
    color = glm::vec3(1.0f, 0.95f, 0.1f);
    renderMode = RenderMode::BASE;
    // mMat.reserve(MAX_JOINTS);
    // for (int i = 0; i < MAX_JOINTS; i++){
    //     mMat.push_back(glm::mat4(1.0f));
    // }
}

Mesh::Mesh(glm::vec3 color){
    this->color = color;
}

Mesh::~Mesh(){
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_pn);
    glDeleteBuffers(1, &VBO_uv);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::setTex(GLuint texID){
    tex = texID;
}

void Mesh::setColor(glm::vec3 color){
    this->color = color;
}

bool Mesh::setMesh(TextureManager* textureManager, const aiMesh* mesh, const aiScene* scene) {
    std::cout << mesh->mName.C_Str() << std::endl;
    vertices.reserve(mesh->mNumVertices);
    // verticesRaw.reserve(mesh->mNumVertices);
    for (int i = 0; i < mesh->mNumVertices; i++){
        Vertex v;

        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        v.normal.x = mesh->mNormals[i].x;
        v.normal.y = mesh->mNormals[i].y;
        v.normal.z = mesh->mNormals[i].z;

        setDefaultJointVal(v);

        vertices.push_back(v);
    }

    indices.reserve(3*(mesh->mNumFaces));
    for (int i = 0; i < mesh->mNumFaces; i++){
        aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3){
            return false;
        }
        for (int j = 0; j < 3; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    // I'm only going to support 1 texture coordinate channel
    if (mesh->HasTextureCoords(0)){
        uvs.reserve(mesh->mNumVertices);

        std::cout << "has texture coordinates" << std::endl;
    
        int dim = mesh->mNumUVComponents[0];
        if (dim != 2){
            std::cerr << "mesh does not use 2d texture coordinates, will truncate to 2d coordinates" << std::endl;
        }
        aiVector3D* uvws = mesh->mTextureCoords[0];

        for (int i = 0; i < mesh->mNumVertices; i++){
            uvs.push_back(glm::vec2(uvws[i].x, uvws[i].y));
        }
    }

    setMaterials(textureManager, mesh, scene);

    setJointVals(mesh);

    return true;
}

void Mesh::setMaterials(TextureManager* textureManager, const aiMesh* mesh, const aiScene *scene) {
    if (mesh->mMaterialIndex < 0){
        std::cout << "no material linked" << std::endl;
        return;
    }
    
    aiColor3D diffuse_color;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // aiMaterialProperty** properties = material->mProperties;

    // for (int i = 0; i < material->mNumProperties; i++){
    //     // std::cout << properties[i]->mKey.C_Str() << std::endl;
    // }

    aiTextureType texDiffuse = aiTextureType_DIFFUSE;

    // std::cout << "texture count" << material->GetTextureCount(texDiffuse) << std::endl;
    if (material->GetTextureCount(texDiffuse) > 0){
        aiString texName;
        material->GetTexture(texDiffuse, 0, &texName);

        if (auto texture = scene->GetEmbeddedTexture(texName.C_Str())) {
            std::cout << "embedded: " << texName.C_Str() << std::endl;
            tex = textureManager->LoadEmbeddedTexture(texture, std::string(texName.C_Str()));
        } else {
            std::cout << "raw: " << texName.C_Str() << std::endl;
            tex = textureManager->LoadTexture(std::string(texName.C_Str()));
        }

        std::cout << tex << std::endl;
        if (tex != 0){
            renderMode = RenderMode::TEXTURE;
        } else {
            std::cout << "texture set to 0" << std::endl;
        }
    }

    if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color)){
        // std::cout << "couldn't get base color";
    } else {
        // std::cout << "base color retrieved" << std::endl;
        color = aiColToGLM(diffuse_color);
    }
}


void Mesh::setSkel(Skeleton* skel){
    this->skel = skel;
}

void Mesh::setJointVals(const aiMesh* mesh){
    if (!(mesh->HasBones())){
        std::cout << "no bones" << std::endl;
        return;
    }

    // std::cout << mesh->mNumBones << std::endl;
    for (int i = 0; i < mesh->mNumBones; i++) {
        int id = -1;
        std::string name = mesh->mBones[i]->mName.C_Str();
        
        if (invBMats.find(name) == invBMats.end()) {
            invBMatInfo newInvBMat;
            id = invBMats.size();

            newInvBMat.id = id;
            // if (name == "mixamorig:LeftArm"){
            //     std::cout << "offsetmat" << std::endl;
            // }
            newInvBMat.invBMat = aiMatToGLM(mesh->mBones[i]->mOffsetMatrix);
            // if (name == "mixamorig:LeftArm"){
            //     std::cout << "done" << std::endl;
            // }
            invBMats[name] = newInvBMat;
        } else {
            id = invBMats[name].id;
            // if (name == "mixamorig:LeftArm"){
            //     std::cout << "done" << std::endl;
            // }
        }

        assert(id != -1);

        auto weights = mesh->mBones[i]->mWeights;
        int numWeights = mesh->mBones[i]->mNumWeights;

        for (int wid= 0; wid < numWeights; wid++){
            int vid = weights[wid].mVertexId;

            assert(vid < vertices.size());

            int sz = vertices[vid].numJoints;
            if (sz < MAX_JOINT_INFLUENCE){
                vertices[vid].jointIDs[sz] = id;
                vertices[vid].weights[sz] = (float) weights[wid].mWeight;

                vertices[vid].numJoints += 1;
            } else {
                std::cout << sz << std::endl;
                std::cout << "exceeded" << std::endl;
            }
        }
    }

}

void Mesh::setDefaultJointVal(Vertex &v){
    for (int i = 0; i < MAX_JOINT_INFLUENCE; i++) {
        v.jointIDs[i] = -1;
        v.weights[i] = 0.0f;
    }
    v.numJoints = 0;
}

// void Mesh::setMMat(glm::mat4 model){
//     this->model = model;
// }

void Mesh::setupBuf(){
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    // Bind to the VAO.
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_pn);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO_pn);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*verticesRaw.size(), verticesRaw.data(), GL_DYNAMIC_DRAW);

    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Bind to the first VBO - We will use it to store the vertex position/normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pn);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    //positions, every Vertex struct, it is the 1st 3 floats starting from the pointer, which starts off at offset 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    //normals, every Vertex struct, it is the 1st 3 floats starting from the pointer, which starts off at offset of what Normal has in the struct Vector 
    //(it is given the label index 1 to use as "location" in vertex shader)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // //jointIDs
    // glEnableVertexAttribArray(2);
    // glVertexAttribIPointer(2, MAX_JOINT_INFLUENCE, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, jointIDs));

    // //weights
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, MAX_JOINT_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

    //jointIDs
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, MAX_JOINT_INFLUENCE/2, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, jointIDs));

    //weights
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, MAX_JOINT_INFLUENCE/2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

    //jointIDs2
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, MAX_JOINT_INFLUENCE/2, GL_INT, sizeof(Vertex), (void*)(offsetof(Vertex, jointIDs) + (4 * sizeof(int))));

    //weights2
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, MAX_JOINT_INFLUENCE/2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, weights) + (4 * sizeof(float))));

    glGenBuffers(1, &VBO_uv);

    // Bind to the second VBO - We will use it to store the uvs
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
    
void Mesh::draw(glm::mat4 model, std::vector<glm::mat4>& mMat, const glm::mat4& viewProjMtx, ShaderManager* shaderManager){
    
    bool animated = (skel != nullptr);
    GLuint shader = shaderManager->getShader(renderMode, animated);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    // actiavte the shader program
    glUseProgram(shader);

    // Bind the VAO
    glBindVertexArray(VAO);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "finalJointMats"), MAX_JOINTS, GL_FALSE, &(mMat[0])[0][0]);
std::cout << "drawing" << std::endl;
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    std::cout << "done drawing" << std::endl;
    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Mesh::update(std::vector<glm::mat4>& mMat){
    if (skel != NULL){
        // std::cout << "yoyo" << std::endl;
        for (auto it = invBMats.begin(); it != invBMats.end(); ++it){
            // std::cout << "id" << std::endl;
            int id = it->second.id;
            // std::cout << id << std::endl;

            // std::cout << glm::to_string(it->second.invBMat) << std::endl;
            // std::cout << glm::to_string(mMat[id]) << std::endl;
            // if (skel == nullptr){
            //     std::cout << "breh" << std::endl;
            // }
            mMat[id] = skel->getWorldMatrix(it->first) * it->second.invBMat;
        }
        // std::cout << "brorbo" << std::endl;
        // std::cout << glm::to_string(mMat[3]) << std::endl;
        // std::cout << "start" << std::endl;
        // for(glm::mat4 m : mMat){
        //     if (m != glm::mat4(1.0f)){
        //         std::cout << "yay" << std::endl;
        //     }
        // }
        // int id = 0;
        // for(Vertex v : vertices){
        //     glm::vec4 totalPos = glm::vec4(0.0f);
        //     glm::vec4 totalNorm = glm::vec4(0.0f);
        //     for(int i = 0; i < MAX_JOINT_INFLUENCE; i++){
        //         // std::cout << v.weights[i] << std::endl;
        //         if (v.jointIDs[i] == -1)
        //         {
        //             // std::cout << "gr" << std::endl;
        //             continue;
        //         }
        //         if (v.jointIDs[i] >= MAX_JOINTS)
        //         {
        //             std::cout << "hmm" << std::endl;
        //             totalPos = glm::vec4(v.position, 1.0f);
        //             totalNorm = glm::vec4(v.normal, 0.0f);
        //             break;
        //         }
        //         glm::vec4 localPos = mMat[v.jointIDs[i]] * glm::vec4(v.position, 1.0f);
        //         //assumes no scale or skew
        //         glm::vec4 localNorm = mMat[v.jointIDs[i]]* glm::vec4(v.normal, 0.0f);
        //         totalPos += v.weights[i] * localPos;
        //         totalNorm += v.weights[i] * localNorm;
        //     }
        //     totalNorm = normalize(totalNorm);
        //     // std::cout << glm::to_string(totalNorm) << std::endl;
        //     // std::cout << glm::to_string(totalPos) << std::endl;
        //     // Vertex v;
        //     // v.position = totalPos;
        //     // v.normal = totalNorm;
        //     if (totalPos == glm::vec4(v.position, 1.0f)){
        //         std::cout << glm::to_string(totalPos) << std::endl;
        //     }
        //     // id++;
        //     // verticesRaw.push_back(v);
        // }

        // //newadd
        // glBindBuffer(GL_ARRAY_BUFFER, VBO_pn);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*verticesRaw.size(), verticesRaw.data(), GL_DYNAMIC_DRAW);
        // //endnewadd
    }
}