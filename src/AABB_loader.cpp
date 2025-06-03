#include "AABB_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cfloat> 
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/common.hpp>

std::map<std::string, AABB> AABB_loader::loadAABBs(const std::string& filepath) {
    std::map<std::string, std::vector<glm::vec3>> objectVertices;
    std::string currentObject;
    std::vector<glm::vec3> tempVertices;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filepath << std::endl;
        return {};
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "o") {
            if (!currentObject.empty() && !tempVertices.empty()) {
                objectVertices[currentObject] = tempVertices;
            }
            tempVertices.clear();
            iss >> currentObject;
        } else if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            tempVertices.emplace_back(x, y, z);
        }
    }

    if (!currentObject.empty() && !tempVertices.empty()) {
        objectVertices[currentObject] = tempVertices;
    }

    std::map<std::string, AABB> result;
    for (const auto& [name, verts] : objectVertices) {
        glm::vec3 minB(FLT_MAX, FLT_MAX, FLT_MAX);
        glm::vec3 maxB(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (const glm::vec3& v : verts) {
            minB = glm::min(minB, v);
            maxB = glm::max(maxB, v);
        }
        
        minB *= 0.001f;
        maxB *= 0.001f;

        result[name] = AABB(minB, maxB);
    }

    return result;
}
