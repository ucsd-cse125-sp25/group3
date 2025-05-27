#pragma once

#include "ModelManager.h"

ModelManager::ModelManager() {

}

ModelManager::~ModelManager() {
    for (auto it = modelMap.begin(); it != modelMap.end(); ++it){
        delete (it->second);
    }
}

void ModelManager::addModel(Model* model) {
    modelMap[model->getModelType()] = model;
} 

Model* ModelManager::getModel(ModelType modelType) {
    if (modelMap.find(modelType) != modelMap.end()){
        return modelMap[modelType];
    } else {
        return nullptr;
    }
}