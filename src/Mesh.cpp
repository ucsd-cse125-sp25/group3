#include "Mesh.h"
#include <iostream>
#include <string>
#include "glm/gtx/string_cast.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Mesh::Mesh(){
    model = glm::mat4(1.0f);
    color = glm::vec3(1.0f, 0.95f, 0.1f);
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

void Mesh::setColor(glm::vec3 color){
    this->color = color;
}

bool Mesh::setMesh(const aiMesh* mesh) {
    vertices.reserve(mesh->mNumVertices);

    for (int i = 0; i < mesh->mNumVertices; i++){
        Vertex v;

        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        v.normal.x = mesh->mNormals[i].x;
        v.normal.y = mesh->mNormals[i].y;
        v.normal.z = mesh->mNormals[i].z;

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

    std::cout << "yello" << std::endl;
    return true;
}

void Mesh::setMMat(glm::mat4 model){
    this->model = model;
}

void Mesh::setupBuf(){
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_pn);
    glGenBuffers(1, &VBO_uv);

    // Bind to the VAO.
    glBindVertexArray(VAO);

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

    // Bind to the second VBO - We will use it to store the uvs
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
    
void Mesh::draw(const glm::mat4& viewProjMtx, GLuint shader){
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Mesh::update(){
    
}