#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Platform {
public:
    Platform(float x, float y, float width, float height, GLuint tex);
    void draw(GLuint shader) const;

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    
    float x, y, width, height;

private:
    
    GLuint texture;


    static GLuint vao;
    static GLuint vbo;
    static bool initialized;
    static void initRenderer();
};
