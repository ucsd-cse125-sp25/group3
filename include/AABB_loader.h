#pragma once

#include "core.h"
#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include "AABB.h"

class AABB_loader {
public:
    static std::map<std::string, AABB> loadAABBs(const std::string& filepath);
};
