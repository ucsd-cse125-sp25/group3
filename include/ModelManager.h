#pragma once

#include "core.h"
#include <unordered_map>
#include "Model.h"
#include "ModelType.h"


class ModelManager {
private:
    std::unordered_map<ModelType, Model*> modelMap;
public:
    ModelManager();
    ~ModelManager();

    void addModel(Model* model);

    Model* getModel(ModelType modelType);
};