#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Skeleton* Window::skel;
Skin* Window::skin;
Animation* Window::anim;
Player* Window::player;
//Cube* Window::cube;

unsigned int Window::fileRender = 0b000;
// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    switch(fileRender) {
        case 0b001:
            shaderProgram = LoadShaders("shaders/shaderp1.vert", "shaders/shaderp1.frag");
            break;
        default:
            // Create a shader program with a vertex shader and a fragment shader.
            shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    }

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

void Window::setRenderFlag(unsigned int flag){
    fileRender = flag;
}

bool Window::initializeObjectsSkelOnly(const char* file) {
    skel = new Skeleton();
    skel->load(file);
    //skel->log();
    skel -> update();
    // Create a cube
    // cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));

    return true;
}

bool Window::initializeObjectsSkinOnly(const char* file) {
    skin = new Skin();
    skin->load(file);
    skin -> update();
    skin->setupBuf();
    // Create a cube
    // cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));

    return true;
}

bool Window::initializeObjects(const char* skelfile, const char *skinfile, const char *animfile) {
    skel = new Skeleton();
    skel->load(skelfile);
    skel->log();
    skel -> update();

    skin = new Skin(skel);
    skin->load(skinfile);
    skin -> update();
    skin->setupBuf();

    anim = new Animation();
    anim->load(animfile);
    
    player = new Player();
    player->setSkel(skel);
    player->setAnim(anim);
    player->initPose();
    // Create a cube
    // cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));

    return true;
}

bool Window::initializeObjectsSkelSkin(const char* skelfile, const char *skinfile) {
    skel = new Skeleton();
    skel->load(skelfile);
    skel->log();
    skel -> update();

    skin = new Skin(skel);
    skin->load(skinfile);
    skin -> update();
    skin->setupBuf();
    // Create a cube
    // cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));

    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete skel;
    delete skin;
    delete anim;
    delete player;
    // delete cube;

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
    Window::resizeCallback(window, width, height);

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
    Cam->Update();

    switch (fileRender){
        case 0b001:
            skel->update();
            break;
        case 0b010:
            skin->update();
            break;
        case 0b011:
            skel->update();
            skin->update();
            break;
        default:
            player->update();
            player->poseSkel();
            skel->update();
            skin->update();
    }
    //cube -> update();
}

void Window::displayCallback(GLFWwindow* window) {
    //get mouse events etc
    glfwPollEvents();
    
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow(); // Show demo window! :)
    if (fileRender != 0b010){
        ShowMyImgui(player, skel);
    }

    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    switch (fileRender){
        case 0b001:
            skel->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
            break;
        default:
            skin->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    //cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

    // (Your code clears your framebuffer, renders your other stuff etc.)
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // (Your code calls glfwSwapBuffers() etc.)

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
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            default:
                break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (!ImGui::GetIO().WantCaptureMouse) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            LeftDown = (action == GLFW_PRESS);
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            RightDown = (action == GLFW_PRESS);
        }
    }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    if (!ImGui::GetIO().WantCaptureMouse){
        int maxDelta = 100;
        int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
        int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

        MouseX = (int)currX;
        MouseY = (int)currY;

        // Move camera
        // NOTE: this should really be part of Camera::Update()
        if (LeftDown) {
            const float rate = 1.0f;
            Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
            Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
        }
        if (RightDown) {
            const float rate = 0.005f;
            float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
            Cam->SetDistance(dist);
        }
    }
}