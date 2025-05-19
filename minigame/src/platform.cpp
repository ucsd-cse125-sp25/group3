#include "platform.h"

Platform::Platform(float x, float y, float width, float height, GLuint tex)
    : x(x), y(y), width(width), height(height), texture(tex) {}

void Platform::draw() const {
    // // glEnable(GL_TEXTURE_2D);
    // // glBindTexture(GL_TEXTURE_2D, texture);
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
    glDisable(GL_TEXTURE_2D); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 0.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    //glDisable(GL_BLEND); 
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);

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
