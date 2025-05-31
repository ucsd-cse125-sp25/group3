#pragma once

#include "core.h"
#include "RenderMode.h"
#include <map>

class ShaderManager {
private:
    std::map<RenderMode, GLuint> nonAnimShaders;
    std::map<RenderMode, GLuint> animShaders;

public:

    ShaderManager();
    ~ShaderManager();

    void addShader(RenderMode renderMode, bool animated, GLuint shader);

    GLuint getShader(RenderMode renderMode, bool animated);
};