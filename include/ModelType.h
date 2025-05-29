#pragma once
#include "core.h"
#include <iostream>
#include <string>

enum class ModelType {
    SecurityGuard, 
    FemaleThief,
    Museum,
    Spider,
    Buddha,
    Bunny,
    Cube
};

std::string modelToString(ModelType model);