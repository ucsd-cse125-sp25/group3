#pragma once
#include "core.h"
#include <iostream>
#include <string>

enum class ModelType {
    SecurityGuard, 
    FemaleThief,
    Artefact_AsianPainting,
    Artefact_BuddhaStatue,
    Artefact_DogSkeleton,
    Artefact_LionStatue,
    Artefact_Michelangelo,
    Artefact_Mondrian,
    Artefact_MonaLisa,
    Artefact_SittingSkeleton,
    Artefact_Skull,
    Artefact_StarryNight,
    Artefact_Torso,
    Museum,
    Item_Baton,
    Item_FlashLight,
    Item_TaserGun,
    Spider,
    Buddha,
    Bunny,
    Cube
};

std::string modelToString(ModelType model);