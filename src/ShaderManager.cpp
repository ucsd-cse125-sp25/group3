#include "ShaderManager.h"
#include "Shader.h"

ShaderManager::ShaderManager() {
    // Create a shader program with a vertex shader and a fragment shader.
    GLuint shaderProgram = LoadShaders("../shaders/shader.vert", "../shaders/shader.frag", false);

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        assert(shaderProgram);
    }

    addShader(RenderMode::BASE, false, shaderProgram);

    GLuint shaderProgram_uv = LoadShaders("../shaders/shader_uv.vert", "../shaders/shader_uv.frag", true);

    if (!shaderProgram_uv) {
        std::cerr << "Failed to initialize shader program with uvs" << std::endl;
        assert(shaderProgram_uv);
    }

    addShader(RenderMode::TEXTURE, false, shaderProgram_uv);

    GLuint shaderProgram_anim = LoadShaders("../shaders/anim_shader_multi.vert", "../shaders/anim_shader.frag", true);

    if (!shaderProgram_anim) {
        std::cerr << "Failed to initialize shader program with anim" << std::endl;
        assert(shaderProgram_anim);
    }

    addShader(RenderMode::BASE, true, shaderProgram_anim);

    GLuint shaderProgram_anim_uv = LoadShaders("../shaders/anim_shader_multi_uv.vert", "../shaders/anim_shader_uv.frag", true);

    if (!shaderProgram_anim_uv) {
        std::cerr << "Failed to initialize shader program with uvs and anim" << std::endl;
        assert(shaderProgram_anim_uv);
    }

    addShader(RenderMode::TEXTURE, true, shaderProgram_anim_uv);

}

ShaderManager::~ShaderManager() {
    for (auto it = nonAnimShaders.begin(); it != nonAnimShaders.end(); ++it) {
        glDeleteProgram(it->second);
    }

    for (auto it = animShaders.begin(); it != animShaders.end(); ++it) {
        glDeleteProgram(it->second);
    }
}

void ShaderManager::addShader(RenderMode renderMode, bool animated, GLuint shader) {
    if (animated) {
        animShaders[renderMode] = shader;
    } else {
        nonAnimShaders[renderMode] = shader;
    }
}

GLuint ShaderManager::getShader(RenderMode renderMode, bool animated) {
    if (animated) {
        assert(animShaders.find(renderMode) != animShaders.end());
        return animShaders[renderMode];
    } else {
        assert(nonAnimShaders.find(renderMode) != nonAnimShaders.end());
        return nonAnimShaders[renderMode];
    }
}