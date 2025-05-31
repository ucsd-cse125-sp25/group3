#include "ModelType.h"

std::string modelToString(ModelType model) {
    switch(model) {
        case ModelType::SecurityGuard:
            return "security_guard";
        case ModelType::FemaleThief:
            return "female_thief";
        case ModelType::Museum:
            return "museum";
        case ModelType::Spider:
            return "spider";
        case ModelType::Bunny:
            return "bunny";
        case ModelType::Cube:
            return "cube";
        default:
            std::cout << "no matching string for model" << std::endl;
            return "";
    }
}