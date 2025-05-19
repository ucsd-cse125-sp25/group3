#ifndef WINDOW_H
#define WINDOW_H

struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();
    void run();
    GLFWwindow* getGLFWwindow() const;

private:
    GLFWwindow* window;
    int width, height;
};

#endif