#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Cube* Window::cube;
Cube* Window::floor;

// Camera Properties
Camera* Cam;
Camera* MiniMapCam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

bool Window::altDown = false;
bool Window::firstMouse = true;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects() {
    // Create a cube
    cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    floor = new Cube(glm::vec3(-8, -2.03, -8), glm::vec3(8, -2.01, 8));

    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete cube;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    // Cam->SetAspect(float(width) / float(height));

    //for minimap
    MiniMapCam = new Camera();
    // MiniMapCam->SetOrtho(-10, 10, -10, 10, 0.1f, 100.0f); 
    // MiniMapCam->SetLookAt(glm::vec3(0, 20, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));


    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    #ifdef __APPLE__
        // macOS: Use framebuffer size to handle Retina displays correctly
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
        // Cam->SetAspect(float(fbWidth) / float(fbHeight));
        Window::width = fbWidth;
        Window::height = fbHeight;
    #else
        // Windows or others: Use default logic-pixel-based callback
        Window::resizeCallback(window, width, height);
    #endif


    // Window::resizeCallback(window, width, height);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    // Cam->Update();

    cube->update();
    Cam->Update(cube->getPosition());
}

void Window::displayCallback(GLFWwindow* window) {
    
    if (cube != nullptr) {
        // cube->handleContinuousInput(window);
        glm::vec3 forward = Cam->GetForwardVector();
        forward.y = 0.0f;  
        forward = glm::normalize(forward);

        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

        cube->handleContinuousInput(window, forward, right);
    }
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    #ifdef __APPLE__
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    #else
        glViewport(0, 0, Window::width, Window::height);
    #endif
    //glViewport(0, 0, Window::width, Window::height);
    cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,false);
    floor->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,true);

    int miniMapSize = 256;
    //glViewport(0, Window::height - miniMapSize, miniMapSize, miniMapSize); 
    #ifdef __APPLE__
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, fbHeight - miniMapSize, miniMapSize, miniMapSize); 
    #else
        glViewport(0, Window::height - miniMapSize, miniMapSize, miniMapSize); 
    #endif
    glm::mat4 viewProj_miniMap = MiniMapCam->GetViewProjectMtx();
    cube->draw(viewProj_miniMap, shaderProgram, false);
    floor->draw(viewProj_miniMap, shaderProgram, true);

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) 
        {
            altDown = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 显示鼠标
        }

        switch (key) 
        {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;


            default:
                if (cube != nullptr) {
                    cube->userInput(key);
                    //cube->handleContinuousInput(window);
                }
                break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) 
        {
            altDown = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }
    }

}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    if (altDown) return;
    
    static float lastX = Window::width / 2.0f;
    static float lastY = Window::height / 2.0f;
    
    float sensitivity = 0.1f;

    if (firstMouse) {
        lastX = currX;
        lastY = currY;
        firstMouse = false;
    }

    float xoffset = currX - lastX;
    float yoffset = currY - lastY; 

    lastX = currX;
    lastY = currY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    
    float newAzimuth = Cam->GetAzimuth() + xoffset;
    float newIncline = glm::clamp(Cam->GetIncline() + yoffset, -89.0f, 89.0f); 

    Cam->SetAzimuth(newAzimuth);
    Cam->SetIncline(newIncline);
}

void Window::applyServerState(const StateUpdatePacket& packet) {
    cube->updateFromPacket(packet);
    Cam->updateFromPacket(packet, false);
    MiniMapCam->updateFromPacket(packet, true);
    altDown = packet.altDown;
}

void Window::render(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    #ifdef __APPLE__
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    #else
        glViewport(0, 0, Window::width, Window::height);
    #endif
    cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,false);
    floor->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,true);

    int miniMapSize = 256;
    //glViewport(0, Window::height - miniMapSize, miniMapSize, miniMapSize); 
    #ifdef __APPLE__
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, fbHeight - miniMapSize, miniMapSize, miniMapSize); 
    #else
        glViewport(0, Window::height - miniMapSize, miniMapSize, miniMapSize); 
    #endif
    glm::mat4 viewProj_miniMap = MiniMapCam->GetViewProjectMtx();
    cube->draw(viewProj_miniMap, Window::shaderProgram, false);
    floor->draw(viewProj_miniMap, Window::shaderProgram, true);


    // Gets events, including input such as keyboard and mouse or window resizing.
    // glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
    
    if (altDown) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
  
    // cube->update();
    // Cam->Update(cube->getPosition());
}