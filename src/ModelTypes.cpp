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
        case ModelType::Artefact_AsianPainting:
            return "horse";
        case ModelType::Artefact_BuddhaStatue:
            return "buddha";
        case ModelType::Artefact_DogSkeleton:
            return "skeleton";
        case ModelType::Artefact_LionStatue:
            return "lion";
        case ModelType::Artefact_Michelangelo:
            return "michaelangelo";
        case ModelType::Artefact_MonaLisa:
            return "monalisa";
        case ModelType::Artefact_Mondrian:
            return "mondrian";
        case ModelType::Artefact_SittingSkeleton:
            return "skeleton_buddha";
        case ModelType::Artefact_Skull:
            return "skull.001";
        case ModelType::Artefact_StarryNight:
            return "starrynight";
        case ModelType::Artefact_Torso:
            return "david";
        default:
            std::cout << "no matching string for model" << std::endl;
            return "";
    }
}