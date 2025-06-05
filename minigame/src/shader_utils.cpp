#include "shader_utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

GLuint compileShader(const char* path, GLenum type) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Shader file not found: " << path << std::endl;
        return 0;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string source = ss.str();
    const char* src = source.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, NULL, info);
        std::cerr << "Shader compile error in " << path << ":\n" << info << std::endl;
    }

    return shader;
}