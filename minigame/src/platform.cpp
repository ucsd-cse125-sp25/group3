#include "platform.h"


GLuint Platform::vao = 0;
GLuint Platform::vbo = 0;
bool Platform::initialized = false;

Platform::Platform(float x, float y, float width, float height, GLuint tex)
    : x(x), y(y), width(width), height(height), texture(tex) {}


void Platform::initRenderer() {
    float vertices[] = {
        // positions    // tex coords
        0.f, 0.f,      0.0f, 0.0f,
        1.f, 0.f,      1.0f, 0.0f,
        1.f, 1.f,      1.0f, 1.0f,

        0.f, 0.f,      0.0f, 0.0f,
        1.f, 1.f,      1.0f, 1.0f,
        0.f, 1.f,      0.0f, 1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    initialized = true;
}

void Platform::draw(GLuint shader) const{ 
    if (!initialized)
        initRenderer();

    glUseProgram(shader);
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader, "backgroundTex"), 0);

    // 构建 model 矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);  
}

float Platform::getX() const {
    return x;
}

float Platform::getY() const {
    return y;
}

float Platform::getWidth() const {
    return width;
}

float Platform::getHeight() const {
    return height;
}
