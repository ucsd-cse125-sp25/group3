#pragma once
#include <GL/glew.h>

class Platform {
public:
    float x, y, width, height;
    GLuint texture;

    Platform(float x, float y, float width, float height, GLuint tex);
    void draw() const;

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
};
