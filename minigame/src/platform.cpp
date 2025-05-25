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
    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0, 0); glVertex2f(x, y);
    // glTexCoord2f(1, 0); glVertex2f(x + width, y);
    // glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
    // glTexCoord2f(0, 1); glVertex2f(x, y + height);
    // glEnd();
    // glDisable(GL_TEXTURE_2D); 

    // glColor3f(1.0f, 0.0f, 0.0f); 
    // glBegin(GL_QUADS);
    // glVertex2f(x, y);
    // glVertex2f(x + width, y);
    // glVertex2f(x + width, y + height);
    // glVertex2f(x, y + height);
    // glEnd();

    // glColor3f(1.0f, 1.0f, 1.0f); 
    // glEnable(GL_TEXTURE_2D);  
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
    // glDisable(GL_TEXTURE_2D); 

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glColor4f(1.0f, 0.0f, 0.0f, 0.0f);

    // glBegin(GL_QUADS);
    // glVertex2f(x, y);
    // glVertex2f(x + width, y);
    // glVertex2f(x + width, y + height);
    // glVertex2f(x, y + height);
    // glEnd();

    // //glDisable(GL_BLEND); 
    // glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    // glEnable(GL_TEXTURE_2D);

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
