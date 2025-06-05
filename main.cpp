#include "Window.h"
#include "core.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// #include "minigame/minigame.h"
#include <GL/glew.h>
// #include "minigame/include/stb_image.h"
#include <iostream>
// #include "minigame/include/window.h"

#include <chrono>

static std::chrono::steady_clock::time_point countdownStartTime;
static const int totalCountdownSeconds = 1 * 10; 
static bool countdownStarted = false;

GameState currentState = START_MENU;
// static MiniGame miniGame;
// static bool miniGameInitialized = false;

const char* GameStateToString(GameState state) {
    switch (state) {
        case START_MENU: return "START MENU";
        case CHARACTER_SELECTION: return "CHARACTER SELECTION";
        case PLAYING: return "PLAYING";
        case IN_MINIGAME: return "IN MINIGAME";
        default: return "UNKNOWN";
    }
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

// Open and read a file, then forward to LoadTextureFromMemory()
bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height)
{
    FILE* f = fopen(file_name, "rb");
    if (f == NULL)
        return false;
    fseek(f, 0, SEEK_END);
    size_t file_size = (size_t)ftell(f);
    if (file_size == -1)
        return false;
    fseek(f, 0, SEEK_SET);
    void* file_data = IM_ALLOC(file_size);
    fread(file_data, 1, file_size, f);
    fclose(f);
    bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
    IM_FREE(file_data);
    return ret;
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
    // https://github.com/Fromager/junicode

    // io.Fonts->AddFontFromFileTTF("../../external/style/fonts/Junicode-Bold.ttf", 32.0f);
    // ImFont* handwritingFont = io.Fonts->AddFontFromFileTTF("../../external/style/fonts/HomemadeApple-Regular.ttf", 28.0f);
    #ifdef _WIN32
        io.Fonts->AddFontFromFileTTF("../../external/style/fonts/Junicode-Bold.ttf", 32.0f);
        ImFont* handwritingFont = io.Fonts->AddFontFromFileTTF("../../external/style/fonts/HomemadeApple-Regular.ttf", 28.0f);
    #else
        io.Fonts->AddFontFromFileTTF("../external/style/fonts/Junicode-Bold.ttf", 32.0f);
        ImFont* handwritingFont = io.Fonts->AddFontFromFileTTF("../external/style/fonts/HomemadeApple-Regular.ttf", 28.0f);
    #endif


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

    
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    #ifdef _WIN32
        bool ret = LoadTextureFromFile("../../external/images/HeistAtTheMuseumTitle.png", &my_image_texture, &my_image_width, &my_image_height);
    #else
        bool ret = LoadTextureFromFile("../external/images/HeistAtTheMuseumTitle.png", &my_image_texture, &my_image_width, &my_image_height);
    #endif
    // bool ret = LoadTextureFromFile("../../external/images/HeistAtTheMuseumTitle.png", &my_image_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
    
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

            // Countdown Timer Window (default display)
            if (countdownStarted) {
                auto now = std::chrono::steady_clock::now();
                int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - countdownStartTime).count();
                int timeLeft = std::max(0, totalCountdownSeconds - elapsed);
            
                int minutes = timeLeft / 60;
                int seconds = timeLeft % 60;
            
                ImVec2 screenSize = ImGui::GetIO().DisplaySize;
                ImVec2 windowSize = ImVec2(200, 50);
                ImVec2 windowPos = ImVec2((screenSize.x - windowSize.x) * 0.5f, 20.0f);
            
                ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
                ImGui::SetNextWindowSize(windowSize);
                ImGui::Begin("##TimeDisplay", nullptr,
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoBackground |
                    ImGuiWindowFlags_NoCollapse);
            
                char timeText[64];
                snprintf(timeText, sizeof(timeText), "Time: %02d:%02d", minutes, seconds);
                float textWidth = ImGui::CalcTextSize(timeText).x;
                ImGui::SetCursorPosX((windowSize.x - textWidth) * 0.5f);
                ImGui::Text("%s", timeText);
            
                ImGui::End();
            }
            

            ImGui::Begin("MINIGAME PLACEHOLDER", nullptr, ImGuiWindowFlags_NoResize);
            
            // if (ImGui::Button("I DIED", ImVec2(120, 40))) {
            //     currentState = IN_MINIGAME;
            //     stateChanged = true; // Flag for state transition
            //     glfwPostEmptyEvent(); // Force frame refresh
            // }
            ImGui::End();
        }

        // UI Rendering
        ImVec2 windowSize = ImGui::GetIO().DisplaySize;
        
        if (currentState == START_MENU) {
            // // Start the ImGui frame
            // ImGui_ImplOpenGL3_NewFrame();
            // ImGui_ImplGlfw_NewFrame();
            // ImGui::NewFrame();

            // // Set window flags
            // ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
            //                                 ImGuiWindowFlags_NoResize |
            //                                 ImGuiWindowFlags_NoMove |
            //                                 ImGuiWindowFlags_NoCollapse |
            //                                 ImGuiWindowFlags_NoBringToFrontOnFocus |
            //                                 ImGuiWindowFlags_NoBackground;

            // ImVec2 windowSize = io.DisplaySize;
            // ImGui::SetNextWindowPos(ImVec2(0, 0));
            // ImGui::SetNextWindowSize(windowSize);
            // ImGui::Begin("StartMenu", nullptr, window_flags);

            // float maxDisplayWidth = windowSize.x * 0.5f;
            // float maxDisplayHeight = windowSize.y * 0.5f;

            // // Calculate the scaling factor based on the smaller dimension
            // float scale = std::min(maxDisplayWidth / my_image_width, maxDisplayHeight / my_image_height);

            // // Compute the new image dimensions
            // float displayWidth = my_image_width * scale;
            // float displayHeight = my_image_height * scale;

            // // Calculate position to center the image
            // float posX = (windowSize.x - displayWidth) * 0.5f;
            // float posY = (windowSize.y - displayHeight) * 0.5f;

            // // Set cursor position and render the image
            // ImGui::SetCursorPos(ImVec2(posX, posY));

            // ImGui::Image((ImTextureID)(intptr_t)my_image_texture, ImVec2(displayWidth, displayHeight));
            // float centerX = windowSize.x * 0.5f;


            // // Start Game button
            // ImGui::SetCursorPos(ImVec2(centerX - 8, 430));
            // if (ImGui::Button("Start Game", ImVec2(160, 50))) {
            //     currentState = CHARACTER_SELECTION;
            // }

            // // Quit button
            // ImGui::SetCursorPos(ImVec2(centerX - 200, 430));
            // if (ImGui::Button("Quit", ImVec2(160, 50))) {
            //     glfwSetWindowShouldClose(window, true);
            // }

            // ImGui::End();
            // Load your parchment background
            GLuint parchmentTexture;
            int parchmentWidth, parchmentHeight;
            #ifdef _WIN32
                bool ok = LoadTextureFromFile("../../external/images/parchment_scroll.png", &parchmentTexture, &parchmentWidth, &parchmentHeight);
            #else
                bool ok = LoadTextureFromFile("../external/images/parchment_scroll.png", &parchmentTexture, &parchmentWidth, &parchmentHeight);
            #endif
            IM_ASSERT(ok);

            // Load a handwriting-style font

            // In your render loop:
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImVec2 screenSize = ImGui::GetIO().DisplaySize;

            // Target width is 40% of the window width (adjust as needed)
            float scaleFactor = 0.4f;
            float parchmentTargetWidth = screenSize.x * scaleFactor;

            // Maintain aspect ratio of the parchment image
            float aspectRatio = (float)parchmentHeight / (float)parchmentWidth;
            float parchmentTargetHeight = parchmentTargetWidth * aspectRatio;

            ImVec2 parchmentSize = ImVec2(parchmentTargetWidth, parchmentTargetHeight);

            // Position it aligned to the center of the left half
            ImVec2 parchmentPos = ImVec2(
                (screenSize.x * 0.25f) - (parchmentTargetWidth * 0.5f), // left center
                (screenSize.y - parchmentTargetHeight) * 0.5f           // vertically center
            );

            // Set up the window
            ImGui::SetNextWindowPos(parchmentPos);
            ImGui::SetNextWindowSize(parchmentSize);
            ImGui::Begin("Parchment", nullptr,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoBackground);

            // Draw the image
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::Image((ImTextureID)(intptr_t)parchmentTexture, parchmentSize);

            // Overlay the buttons
            ImGui::SetCursorPos(ImVec2(parchmentSize.x * 0.3f, parchmentSize.y * 0.2f));
            ImGui::PushFont(handwritingFont);

            ImGui::Text("Main Menu");


            // Overlay the buttons
            // ImGui::PushFont(handwritingFont);

            const char* title = "Main Menu";
            ImVec2 textSize = ImGui::CalcTextSize(title);

            // Get current window width
            float windowWidth = ImGui::GetWindowSize().x;

            // Center the text by moving the cursor
            ImGui::SetCursorPosX((windowWidth - textSize.x) * 0.5f);

            // Render the text
            ImGui::Text("%s", title);

            // Define text colors
            ImVec4 normalColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // black
            ImVec4 hoverColor  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // white

            // Get draw list for custom rendering
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // --- Button 1: Start Game ---
            const char* startLabel = "Start Game";
            ImVec2 startSize = ImGui::CalcTextSize(startLabel);
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - startSize.x) * 0.5f);

            if (ImGui::InvisibleButton("##start", startSize)) {
                // Handle start game action
                currentState = CHARACTER_SELECTION;
            }
            ImVec2 startPos = ImGui::GetItemRectMin();
            ImVec4 startColor = ImGui::IsItemHovered() ? hoverColor : normalColor;
            draw_list->AddText(startPos, ImGui::ColorConvertFloat4ToU32(startColor), startLabel);

            // --- Spacing ---
            ImGui::Dummy(ImVec2(0.0f, 20.0f)); // vertical space between buttons

            // --- Button 2: Options ---
            const char* optionsLabel = "Options";
            ImVec2 optionsSize = ImGui::CalcTextSize(optionsLabel);
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - optionsSize.x) * 0.5f);

            if (ImGui::InvisibleButton("##options", optionsSize)) {
                // Handle options click
            }
            ImVec2 optionsPos = ImGui::GetItemRectMin();
            ImVec4 optionsColor = ImGui::IsItemHovered() ? hoverColor : normalColor;
            draw_list->AddText(optionsPos, ImGui::ColorConvertFloat4ToU32(optionsColor), optionsLabel);

            // --- Spacing ---
            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            // --- Button 3: Quit ---
            const char* quitLabel = "Quit";
            ImVec2 quitSize = ImGui::CalcTextSize(quitLabel);

            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - quitSize.x) * 0.5f);


            if (ImGui::InvisibleButton("##quit", quitSize)) {
                glfwSetWindowShouldClose(window, true);
            }
            ImVec2 quitPos = ImGui::GetItemRectMin();
            ImVec4 quitColor = ImGui::IsItemHovered() ? hoverColor : normalColor;
            draw_list->AddText(quitPos, ImGui::ColorConvertFloat4ToU32(quitColor), quitLabel);



            ImGui::PopFont();

            ImGui::End();

            // Mandatory ImGui frame termination
            ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Current Game State: %s", GameStateToString(currentState));
            ImGui::End();
        } else if (currentState == CHARACTER_SELECTION) {
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
            ImGuiWindowFlags sidePanelFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

            ImVec2 windowSize = io.DisplaySize;
            float panelWidth = 360.0f;  // adjust width to your liking
            float panelHeight = windowSize.y;

            // Fix position to right side
            ImGui::SetNextWindowPos(ImVec2(windowSize.x - panelWidth, 0));
            ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));

            // This window can collapse, but can't be moved or resized
            // ImGui::Begin("Character Customization", nullptr, sidePanelFlags);

            //             if (ImGui::Button("Confirm", ImVec2(120, 40))) {
            //         currentState = PLAYING;
            //         stateChanged = true; // Flag for state transition
            //         glfwPostEmptyEvent(); // Force frame refresh
            //     }
            //             ImGui::End();


            ImGui::Begin("Character Customization", nullptr, ImGuiWindowFlags_NoResize);

            ImGui::Text("🧍 Customize Your Character");
            ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.43f, 0.35f, 0.27f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.55f, 0.44f, 0.32f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.37f, 0.29f, 0.21f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.98f, 0.92f, 0.84f, 1.00f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, 10));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 14));


            ImGui::Separator();
            ImGui::Spacing();

            // === Left Column ===


            // === Right Column ===
            ImGui::BeginChild("Options", ImVec2(0, 500), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

            const char* tabs[] = { "👔 Outfit", "🎩 Hat", "🎒 Accessory" };
            static int tab = 0;
            for (int i = 0; i < 3; ++i) {
                if (i > 0) ImGui::SameLine();
                if (ImGui::Selectable(tabs[i], tab == i)) tab = i;
            }
            ImGui::Separator();

            const char* outfitOptions[] = { "Casual", "Stealth", "Fancy", "Rugged" };
            const char* hatOptions[] = { "None", "Cap", "Hood", "Crown" };
            const char* accessoryOptions[] = { "Backpack", "Cape", "Glasses" };

            const char** options = nullptr;
            int count = 0;
            if (tab == 0) { options = outfitOptions; count = IM_ARRAYSIZE(outfitOptions); }
            else if (tab == 1) { options = hatOptions; count = IM_ARRAYSIZE(hatOptions); }
            else { options = accessoryOptions; count = IM_ARRAYSIZE(accessoryOptions); }

            for (int i = 0; i < count; ++i) {
                if (ImGui::Button(options[i], ImVec2(200, 40))) {
                    // Handle selection
                }
            }

            ImGui::EndChild();

            ImGui::Spacing();
            if (ImGui::Button("◀ Back")) currentState = START_MENU;
            ImGui::SameLine();
            if (ImGui::Button("Randomize")) { /* randomize logic */ }
            ImGui::SameLine();
            // if (ImGui::Button("Confirm")) {currentState = PLAYING;}
            if (ImGui::Button("Confirm", ImVec2(120, 40))) {
                currentState = PLAYING;
                stateChanged = true; // Flag for state transition
                glfwPostEmptyEvent(); // Force frame refresh

                // start timer
                countdownStarted = true;
                countdownStartTime = std::chrono::steady_clock::now();
            }
            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(4);

            // ImGui::End();
            ImGui::End();

            // Mandatory ImGui frame termination
            ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Current Game State: %s", GameStateToString(currentState));
            ImGui::End();
        }
        
        // if (currentState == IN_MINIGAME) {

        //         // std::cout << "minigame" << std::endl;
        //     if (!miniGameInitialized) {
        //         miniGame.init(window);
        //         miniGameInitialized = true;
        //         std::cout << "minigame initialized" << std::endl;
        //     }

        //     //std::cout << "minigame initialized" << std::endl;
        //     miniGame.update(window);
        //     miniGame.render();

        //     if (miniGame.isFinished()) {
        //         miniGame.cleanup();
        //         currentState = PLAYING; 
        //         miniGameInitialized = false;
        //     }
        // }

        if (currentState == IN_MINIGAME){
            // Window::displayCallback(window);
        }
        Window::idleCallback();
        if (currentState != IN_MINIGAME ){
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
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