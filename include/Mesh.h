#pragma once

#include <vector>
#include "core.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

class Mesh {
private:
    GLuint VAO;
    GLuint VBO_pn, VBO_uv, EBO;

    glm::mat4 model;
    glm::vec3 color;

    std::vector<Vertex> vertices;

    std::vector<glm::vec2> uvs;
    //which vertices correspond to triangle
    std::vector<unsigned int> indices;

public:
    Mesh();
    Mesh(glm::vec3 color);
    ~Mesh();

    bool load(const std::string & file);

    void setColor(glm::vec3 color);

    bool setMesh(const aiMesh* mesh);

    void setMMat(glm::mat4 model);
    
    void setupBuf();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();
};