#include "Window.h"
#include "core.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


GameState currentState = START_MENU;

void error_callback(int error, const char* description) {
    std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window) {
    glfwSetErrorCallback(error_callback);
    glfwSetWindowSizeCallback(window, Window::resizeCallback);
    glfwSetKeyCallback(window, Window::keyCallback);
    glfwSetMouseButtonCallback(window, Window::mouse_callback);
    glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions() {
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << std::endl;

#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(void) {
    GLFWwindow* window = Window::createWindow(800, 600);
    if (!window) exit(EXIT_FAILURE);

    print_versions();
    setup_callbacks(window);
    setup_opengl_settings();

    if (!Window::initializeProgram()) exit(EXIT_FAILURE);
    if (!Window::initializeObjects()) exit(EXIT_FAILURE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    static int selectedCharacter = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Set background based on game state
        if (currentState == START_MENU) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fully black
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Window::displayCallback(window);

        // ----- START MENU -----
        if (currentState == START_MENU) {
            ImVec2 windowSize = io.DisplaySize;

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(windowSize);

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                            ImGuiWindowFlags_NoResize |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoCollapse |
                                            ImGuiWindowFlags_NoBringToFrontOnFocus |
                                            ImGuiWindowFlags_NoBackground;

            ImGui::Begin("StartMenu", nullptr, window_flags);

            float centerX = windowSize.x * 0.5f;
            float centerY = windowSize.y * 0.5f;

            ImGui::SetCursorPos(ImVec2(centerX - 100, centerY - 100));
            ImGui::Text("Welcome to Heist of the Museum!");

            ImGui::SetCursorPos(ImVec2(centerX - 50, centerY - 40));
            if (ImGui::Button("Start Game", ImVec2(100, 40))) {
                currentState = CHARACTER_SELECTION;
            }

            ImGui::SetCursorPos(ImVec2(centerX - 50, centerY + 10));
            if (ImGui::Button("Quit", ImVec2(100, 40))) {
                glfwSetWindowShouldClose(window, true);
            }

            ImGui::End();
        }

        // ----- CHARACTER SELECTION -----
        else if (currentState == CHARACTER_SELECTION) {
            ImGui::Begin("Select Your Character");

            if (ImGui::RadioButton("Thief", selectedCharacter == 0)) selectedCharacter = 0;
            if (ImGui::RadioButton("Guard", selectedCharacter == 1)) selectedCharacter = 1;
            if (ImGui::RadioButton("Something", selectedCharacter == 2)) selectedCharacter = 2;

            if (ImGui::Button("Start Game")) {
                currentState = PLAYING;
                // Optionally: setupCharacter(selectedCharacter);
            }

            ImGui::End();

            if (Window::cube != nullptr) {
                switch (selectedCharacter) {
                    case 1: Window::cube->type = Cube::CHARACTER_1; break;
                    case 2: Window::cube->type = Cube::CHARACTER_2; break;
                    case 3: Window::cube->type = Cube::CHARACTER_3; break;
                    case 4: Window::cube->type = Cube::CHARACTER_4; break;
                    default: Window::cube->type = Cube::CHARACTER_1; break;
                }
            }
        }

        // ----- MAIN GAME -----
        else {
            ImGui::Begin("Tool Bar");

            const char* abilities[] = { "None", "Invisibility", "Speed Up" };
            static int currentSelection = 0;

            if (ImGui::Combo("Select Ability", &currentSelection, abilities, IM_ARRAYSIZE(abilities))) {
                Window::currentAbility = static_cast<AbilityType>(currentSelection);
            }

            ImGui::End();
        }

        // Update objects
        Window::idleCallback();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    Window::cleanUp();
    glfwDestroyWindow(window);
    glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    exit(EXIT_SUCCESS);
}
