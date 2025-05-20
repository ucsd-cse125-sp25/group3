#include "Window.h"
#include "core.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GameState currentState = START_MENU;

const char* GameStateToString(GameState state) {
    switch (state) {
        case START_MENU: return "START_MENU";
        case CHARACTER_SELECTION: return "CHARACTER_SELECTION";
        case PLAYING: return "PLAYING";
        default: return "UNKNOWN";
    }
}


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
}

void print_versions() {
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << std::endl;
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

void CenterText(const char* text) {
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(text).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("%s", text);
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
    ImGuiIO& io = ImGui::GetIO();
     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    
    // Font and style setup
    io.Fonts->AddFontFromFileTTF("external/style/fonts/Bauhaus93.ttf", 32.0f);
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Opaque background
    style.Colors[ImGuiCol_Button] = ImVec4(0.7f, 0.6f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.8f, 0.7f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.9f, 0.8f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.98f, 0.9f, 1.0f);
    style.FrameRounding = 12.0f;
    style.WindowRounding = 16.0f;
    style.FramePadding = ImVec2(14, 10);
    style.ItemSpacing = ImVec2(16, 12);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    

    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Clear buffers
        if (currentState == START_MENU || currentState == CHARACTER_SELECTION) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        } else {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        


    // 2. Update game state BEFORE rendering
    static bool stateChanged = false;
    if (currentState == CHARACTER_SELECTION && stateChanged) {
        glfwWaitEventsTimeout(0.1); // Add small delay for state transition
        stateChanged = false;
    }

    // 3. Clear buffers aggressively during state transitions
    // if (currentState == PLAYING) {
    //     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // }



        // Render game scene ONLY when playing
        if (currentState == PLAYING) {
            ImVec2 windowSize = io.DisplaySize;
            ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
            Window::displayCallback(window);

            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 200, ImGui::GetIO().DisplaySize.y - 100));
    ImGui::SetNextWindowSize(ImVec2(400, 80));

    ImGui::Begin("Toolbar", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));

    static int selectedSlot = 0;
    const char* abilities[] = { "None", "Invisibility", "Speed", "Trap", "Scan" };
    int slotCount = IM_ARRAYSIZE(abilities);

    for (int i = 0; i < slotCount; ++i) {
        if (i > 0) ImGui::SameLine();

        if (i == selectedSlot) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.7f, 0.3f, 1.0f)); // highlight
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        }

        std::string label = "[" + std::string(abilities[i]) + "]";
        if (ImGui::Button(label.c_str(), ImVec2(64, 64))) {
            selectedSlot = i;
            Window::currentAbility = static_cast<AbilityType>(i); // your own enum
        }

        ImGui::PopStyleColor();
    }

    ImGui::PopStyleVar(2);
  

    ImGui::End();

ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
ImGui::Text("Current Game State: %s", GameStateToString(currentState));
ImGui::End();



        }

        // UI Rendering
        ImVec2 windowSize = ImGui::GetIO().DisplaySize;
        
        if (currentState == START_MENU) {
// Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                            ImGuiWindowFlags_NoResize |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoCollapse |
                                            ImGuiWindowFlags_NoBringToFrontOnFocus |
                                            ImGuiWindowFlags_NoBackground;
           

            // [Your menu UI code here]
            ImVec2 windowSize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("StartMenu", nullptr, window_flags);

    float centerX = windowSize.x * 0.5f;

    ImGui::SetCursorPos(ImVec2(centerX - 250, 120));
    ImGui::PushFont(io.Fonts->Fonts[0]);
    ImGui::Text("HEUM");
    ImGui::PopFont();

    ImGui::SetCursorPos(ImVec2(centerX - 120, 200));

    ImGui::SetCursorPos(ImVec2(centerX - 70, 260));
    if (ImGui::Button("Start Game", ImVec2(160, 50))) {
        currentState = CHARACTER_SELECTION;
    }

    ImGui::SetCursorPos(ImVec2(centerX - 70, 330));
    if (ImGui::Button("Quit", ImVec2(160, 50))) {
        glfwSetWindowShouldClose(window, true);
    }

            
            ImGui::End(); 

             // Mandatory ImGui frame termination
             ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
ImGui::Text("Current Game State: %s", GameStateToString(currentState));
ImGui::End();

        }
        else if (currentState == CHARACTER_SELECTION) {
            // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

            ImGui::SetNextWindowSize(windowSize);
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            // ImGui::Begin("CharacterSelect", nullptr,
            //     ImGuiWindowFlags_NoTitleBar |
            //     ImGuiWindowFlags_NoResize |
            //     ImGuiWindowFlags_NoMove);

            ImGui::Begin("Character Customization", nullptr, ImGuiWindowFlags_NoResize);
            if (ImGui::Button("Confirm", ImVec2(120, 40))) {
        currentState = PLAYING;
        stateChanged = true; // Flag for state transition
        glfwPostEmptyEvent(); // Force frame refresh
    }
            ImGui::End();


//             ImGui::Begin("Character Customization", nullptr, ImGuiWindowFlags_NoResize);

// ImGui::Text("🧍 Customize Your Character");
// ImGui::Separator();
// ImGui::Spacing();

// ImGui::Columns(2, nullptr, false);

// // === Left Column ===
// ImGui::BeginChild("Preview", ImVec2(350, 500), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
// ImGui::Text("🖼 Character Preview");
// ImGui::Dummy(ImVec2(300, 400));
// ImGui::Text("Render goes here");
// ImGui::EndChild();

// ImGui::NextColumn();

// // === Right Column ===
// ImGui::BeginChild("Options", ImVec2(0, 500), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

// const char* tabs[] = { "👔 Outfit", "🎩 Hat", "🎒 Accessory" };
// static int tab = 0;
// for (int i = 0; i < 3; ++i) {
//     if (i > 0) ImGui::SameLine();
//     if (ImGui::Selectable(tabs[i], tab == i)) tab = i;
// }
// ImGui::Separator();

// const char* outfitOptions[] = { "Casual", "Stealth", "Fancy", "Rugged" };
// const char* hatOptions[] = { "None", "Cap", "Hood", "Crown" };
// const char* accessoryOptions[] = { "Backpack", "Cape", "Glasses" };

// const char** options = nullptr;
// int count = 0;
// if (tab == 0) { options = outfitOptions; count = IM_ARRAYSIZE(outfitOptions); }
// else if (tab == 1) { options = hatOptions; count = IM_ARRAYSIZE(hatOptions); }
// else { options = accessoryOptions; count = IM_ARRAYSIZE(accessoryOptions); }

// for (int i = 0; i < count; ++i) {
//     if (ImGui::Button(options[i], ImVec2(200, 40))) {
//         // Handle selection
//     }
// }

// ImGui::EndChild();

// ImGui::Spacing();
// if (ImGui::Button("◀ Back")) currentState = START_MENU;
// ImGui::SameLine();
// if (ImGui::Button("Randomize")) { /* randomize logic */ }
// ImGui::SameLine();
// // if (ImGui::Button("Confirm")) {currentState = PLAYING;}
// if (ImGui::Button("Confirm", ImVec2(120, 40))) {
//         currentState = PLAYING;
//         stateChanged = true; // Flag for state transition
//         glfwPostEmptyEvent(); // Force frame refresh
//     }

// ImGui::End();

            
//             ImGui::End(); // Crucial for proper frame ending

             // Mandatory ImGui frame termination
             ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
ImGui::Text("Current Game State: %s", GameStateToString(currentState));
ImGui::End();

        
        }



       

         
        Window::idleCallback();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    glfwSwapBuffers(window);

    }


    // Cleanup
    Window::cleanUp();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}