#include "Window.h"
#include "Mesh.h"
#include "TextureManager.h"
// #include <assimp/cimport.h>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
#include <string>

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Cube* Window::cube;
Cube* Window::floor;
Character* Window::character;

Scene* Window::scene;
Model* Window::model;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;
GLuint Window::shaderProgram_uv;

TextureManager* textureManager;

bool Window::altDown = false;
bool Window::firstMouse = true;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("../shaders/shader.vert", "../shaders/shader.frag", false);

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    shaderProgram_uv = LoadShaders("../shaders/shader_uv.vert", "../shaders/shader_uv.frag", true);

    if (!shaderProgram_uv) {
        std::cerr << "Failed to initialize shader program with uvs" << std::endl;
        return false;
    }

    textureManager = new TextureManager();

    return true;
}

bool Window::initializeObjects() {
    // Create a cube
    cube = new Cube();
    
    scene = new Scene();
    if (!scene->load("../models/bunny.ply")){
        std::cerr << "Failed to load ply: Bunny" << std::endl;
        return false;
    };
    Model* m = scene->getModel(0);
    Mesh* mesh = m->getMesh(0);
    mesh->setColor(glm::vec3(1.0f, 0.0f, 1.0f));
    mesh->setTex(textureManager->LoadTexture("../textures/wall.jpg"));
    m->setMMat(glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)));
    scene -> update();
    scene-> setupBufAll();

    model = new Model();
    model->setMMat(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f)));
    if (!model->load("../models/buddha.ply")){
        std::cerr << "Failed to load ply: Buddha" << std::endl;
        return false;
    }
    model->getMesh(0)->setTex(textureManager->LoadTexture("../textures/wall.jpg"));
    model -> update();
    model -> setupBuf();

    character = new Character(model);
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    floor = new Cube(glm::vec3(-8, -2.03, -8), glm::vec3(8, -2.01, 8));

    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete cube;

    delete floor;
    delete scene;
    delete model;
    delete character;

    delete textureManager;
    
    // Delete the shader program.
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram_uv);
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
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    #ifdef __APPLE__
        // macOS: Use framebuffer size to handle Retina displays correctly
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
        Cam->SetAspect(float(fbWidth) / float(fbHeight));
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
    Cam->Update(cube->getPosition());

    // cube->update();
    character->update();

    scene->update();
    // model->update();
}

void Window::displayCallback(GLFWwindow* window) {

    // if (cube != nullptr) {
    //     // cube->handleContinuousInput(window);
    //     glm::vec3 forward = Cam->GetForwardVector();
    //     forward.y = 0.0f;  
    //     forward = glm::normalize(forward);

    //     glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

    //     cube->handleContinuousInput(window, forward, right);
    //     // character->handleInput(window, forward,right);
    // }
    if (character != nullptr) {
        glm::vec3 forward = Cam->GetForwardVector();
        forward.y = 0.0f;
        forward = glm::normalize(forward);
    
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    
        character->handleInput(window, forward, right);
    }
    
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    // cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,false);
    floor->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,true);

    scene->draw(Cam->GetViewProjectMtx(), Window::shaderProgram_uv);
    // model->draw(Cam->GetViewProjectMtx(), Window::shaderProgram_uv);
    character->draw(Cam->GetViewProjectMtx(),shaderProgram_uv);

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
                // if (cube != nullptr) {
                //     cube->userInput(key);
                //     //cube->handleContinuousInput(window);
                // }
                // break;
                if (character != nullptr) {
                    if (key == GLFW_KEY_SPACE) {
                        character->triggerJump();
                    }
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
    float yoffset = currY - lastY; // 注意这里是 lastY - ypos，y 是反的

    lastX = currX;
    lastY = currY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    
    float newAzimuth = Cam->GetAzimuth() + xoffset;
    float newIncline = glm::clamp(Cam->GetIncline() + yoffset, -89.0f, 89.0f); // 防止翻转

    Cam->SetAzimuth(newAzimuth);
    Cam->SetIncline(newIncline);
}