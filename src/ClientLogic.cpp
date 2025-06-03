#include "ClientLogic.h" 

ImVec2 client_logic::displaySize;
ImGuiIO* client_logic::io;
ImFont* client_logic::handwritingFont;
bool client_logic::availableChars[4];
MiniGame client_logic::miniGame;
bool client_logic::miniGameInitialized = false;
std::vector<Platform> client_logic::miniGamePlatformsServer;
// ImGuiIO& client_logic::io;
std::vector<std::unique_ptr<Packet>> client_logic::pendingPackets;

void client_logic::setMinigamePlatformsServer(const InitMinigamePacket& initMinigamePacket) {
    miniGamePlatformsServer.clear();
    for(int i = 0; i < initMinigamePacket.numPlatforms; i++) {
        Platform plat = Platform(initMinigamePacket.platformX[i], initMinigamePacket.platformY[i], initMinigamePacket.platformWidth[i], initMinigamePacket.platformHeight[i], 0);
        miniGamePlatformsServer.emplace_back(plat);
        std::cout << "Received platform: " << initMinigamePacket.platformX[i] << ", " << initMinigamePacket.platformY[i] << ", " << initMinigamePacket.platformWidth[i] << ", " << initMinigamePacket.platformHeight[i] << std::endl;
    }
}

void client_logic::updateAvailableChars(GuiUpdatePacket& packet) {
    std::cout << "Updating available characters" << std::endl;
    for (int i=0; i<4; i++) {
        availableChars[i] = packet.availableChars[i];
    }
}

void client_logic::error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
}

void client_logic::setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void client_logic::setup_callbacks(GLFWwindow* window) {
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);

    // Set the key callback.
    // glfwSetKeyCallback(window, Window::keyCallback);
    glfwSetKeyCallback(window, client_logic::keyCallBack);
    // Set the mouse and cursor callbacks
    glfwSetMouseButtonCallback(window, Window::mouse_callback);
    glfwSetCursorPosCallback(window, client_logic::cursor_callback);
}

void client_logic::handleUserInput(GLFWwindow* window) {
    
    KeyType key = KeyType::NONE;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        key = KeyType::KEY_W;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        key = KeyType::KEY_S;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        key = KeyType::KEY_D;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        key = KeyType::KEY_A;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        key = KeyType::KEY_T;

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        key = KeyType::KEY_G;

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        key = KeyType::KEY_K;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        key = KeyType::KEY_L;
    
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        key = KeyType::KEY_E;

    if (key != KeyType::NONE) {
        auto packet = std::make_unique<KeyPacket>();
        packet->packet_type = KEY_INPUT;
        packet->key_type = key;
        pendingPackets.push_back(std::move(packet));
    }
    
}

void client_logic::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // printf("running callback\n");
    // Packet packet;
    // KeyType keyType;

    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) 
        {
            // altDown = true;
            // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 显示鼠标
            auto packet = std::make_unique<KeyPacket>();
            packet->packet_type = KEY_INPUT;
            packet->key_type = KeyType::KEY_ALT_PRESS;
            pendingPackets.push_back(std::move(packet));
        }

        switch (key) 
        {
            case GLFW_KEY_ESCAPE: {
                // Close the window. This causes the program to also terminate.
                // pendingKeys.push_back(KeyType::KEY_ESC);
                // packet.payload.resize(1);
                // memcpy(packet.payload.data(), &key, sizeof(key));

                auto packet = std::make_unique<EndGamePacket>();
                packet->packet_type = END_GAME;
                pendingPackets.push_back(std::move(packet));
                break;
            }
            case GLFW_KEY_R: {
                // resetCamera();
                auto packet = std::make_unique<KeyPacket>();
                packet->packet_type = KEY_INPUT;
                packet->key_type = KeyType::KEY_R;
                pendingPackets.push_back(std::move(packet));
                break;
            }
            case GLFW_KEY_SPACE: {
                auto packet = std::make_unique<KeyPacket>();
                packet->packet_type = KEY_INPUT;
                packet->key_type = KeyType::KEY_SPACE;
                pendingPackets.push_back(std::move(packet));
                
                // pendingKeys.push_back(KeyType::KEY_SPACE);
                // printf("size: %zu\n", pendingKeys.size());
                    // cube->userInput(key);
                    //cube->handleContinuousInput(window);
                
                break;
            }
            case GLFW_KEY_F: {
                auto packet = std::make_unique<KeyPacket>();
                packet->packet_type = KEY_INPUT;
                packet->key_type = KeyType::KEY_F;
                pendingPackets.push_back(std::move(packet));
                break;
            }
            default:
                break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) 
        {
            // altDown = false;
            // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // firstMouse = true;
            auto packet = std::make_unique<KeyPacket>();
            packet->packet_type = KEY_INPUT;
            packet->key_type = KeyType::KEY_ALT_RELEASE;
            pendingPackets.push_back(std::move(packet));
        }
    }
}

void client_logic::cursor_callback(GLFWwindow* window, double currX, double currY) {
    auto packet = std::make_unique<CursorPacket>();
    packet->packet_type = CURSOR_EVENT;
    packet->currX = currX;
    packet->currY = currY;
    pendingPackets.push_back(std::move(packet));
}

bool client_logic::LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height) {
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
bool client_logic::LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height) {
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

void client_logic::setStartPage(GameState currState) {
    GLuint parchmentTexture;
    int parchmentWidth, parchmentHeight;
    bool ok = LoadTextureFromFile("../external/images/parchment_scroll.png", &parchmentTexture, &parchmentWidth, &parchmentHeight);
    IM_ASSERT(ok);

    // Load a handwriting-style font
    // ImFont* handwritingFont = io.Fonts->AddFontFromFileTTF("../external/style/fonts/HomemadeApple-Regular.ttf", 28.0f);

    // In your render loop:
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (currState == INIT) return;

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
        // currentState = CHARACTER_SELECTION;
        auto packet = std::make_unique<KeyPacket>();
        packet->packet_type = KEY_INPUT;
        packet->key_type = KeyType::MENU_START;
        pendingPackets.push_back(std::move(packet));
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
        // glfwSetWindowShouldClose(window, true);
        auto packet = std::make_unique<EndGamePacket>();
        packet->packet_type = END_GAME;
        pendingPackets.push_back(std::move(packet));
    }
    ImVec2 quitPos = ImGui::GetItemRectMin();
    ImVec4 quitColor = ImGui::IsItemHovered() ? hoverColor : normalColor;
    draw_list->AddText(quitPos, ImGui::ColorConvertFloat4ToU32(quitColor), quitLabel);

    ImGui::PopFont();

    ImGui::End();

    // Mandatory ImGui frame termination
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Current Game State: %s", "START MENU");
    ImGui::End();
}

void client_logic::setCharacterSelectPage(GameState currState) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    // ImGui::Begin("CharacterSelect", nullptr,
    //     ImGuiWindowFlags_NoTitleBar |
    //     ImGuiWindowFlags_NoResize |
    //     ImGuiWindowFlags_NoMove);
    ImGuiWindowFlags sidePanelFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    ImVec2 windowSize = io->DisplaySize;
    float panelWidth = 360.0f;  // adjust width to your liking
    float panelHeight = windowSize.y;

    // Fix position to right side
    ImGui::SetNextWindowPos(ImVec2(windowSize.x - panelWidth, 0));
    ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));

    ImGui::BeginDisabled(currState == WAITING);

    ImGui::Begin("Toolbar", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));

    static int selectedSlot = -1;

    if (selectedSlot != -1 && !availableChars[selectedSlot]) {
        selectedSlot = -1;
    }
    const char* abilities[] = { "Thief 1", "Thief 2", "Thief 3", "Security Guard" };
    int slotCount = IM_ARRAYSIZE(abilities);
    static CharacterType selCharacter;
    // ImGui::EndDisabled();

    for (int i = 0; i < slotCount; ++i) {
        if (i > 0) ImGui::SameLine();

        if (i == selectedSlot) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.7f, 0.3f, 1.0f)); // highlight
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        }

        std::string label = "[" + std::string(abilities[i]) + "]";
        ImGui::BeginDisabled(!availableChars[i]);
        if (ImGui::Button(label.c_str(), ImVec2(64, 64))) {
            selectedSlot = i;
            selCharacter = static_cast<CharacterType>(i); // your own enum
        }
        ImGui::EndDisabled();
        ImGui::PopStyleColor();
    }

    ImGui::PopStyleVar(2);
    ImGui::End();
    
    // This window can collapse, but can't be moved or resized
    // ImGui::Begin("Character Customization", nullptr, sidePanelFlags);

    //             if (ImGui::Button("Confirm", ImVec2(120, 40))) {
    //         currentState = PLAYING;
    //         stateChanged = true; // Flag for state transition
    //         glfwPostEmptyEvent(); // Force frame refresh
    //     }
    //             ImGui::End();


    // ImGui::Begin("Character Customization", nullptr, ImGuiWindowFlags_NoResize);
    // ImGui::BeginDisabled(currState == WAITING);

    ImGui::Begin("OPTIONS PLACEHOLDER", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::Text("🧍 Customize Your Character");
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.43f, 0.35f, 0.27f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.55f, 0.44f, 0.32f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.37f, 0.29f, 0.21f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.98f, 0.92f, 0.84f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 14));


    // ImGui::Separator();
    // ImGui::Spacing();

    // === Left Column ===


    // === Right Column ===
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
    
    
        
    if (ImGui::Button("◀ Back")) {
        auto packet = std::make_unique<KeyPacket>();
        packet->packet_type = KEY_INPUT;
        packet->key_type = KeyType::CHAR_SEL_BACK;
        pendingPackets.push_back(std::move(packet));
    }
    ImGui::SameLine();
    if (ImGui::Button("Randomize")) { /* randomize logic */ }
    ImGui::SameLine();
    // if (ImGui::Button("Confirm")) {currentState = PLAYING;}
    if (ImGui::Button("Confirm", ImVec2(120, 40))) {
        // currentState = PLAYING;
        // stateChanged = true; // Flag for state transition
        // glfwPostEmptyEvent(); // Force frame refresh
        auto packet = std::make_unique<InitPacket>();
        packet->packet_type = INIT_CONNECTION;
        packet->character = selCharacter;
        packet->windowWidth = Window::width;
        packet->windowHeight = Window::height;
        pendingPackets.push_back(std::move(packet));
    }
    // ImGui::EndDisabled();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(4);

    // ImGui::End();
    ImGui::End();
    ImGui::EndDisabled();
    // Mandatory ImGui frame termination
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Current Game State: %s", "Character Select");
    ImGui::End();
}

void client_logic::setMainGameWindow(GLFWwindow* window) {
    ImVec2 windowSize = io->DisplaySize;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    Window::render(window);
    // Window::displayCallback(window);

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 200, ImGui::GetIO().DisplaySize.y - 100));
    ImGui::SetNextWindowSize(ImVec2(400, 80));

    // ImGui::Begin("Toolbar", nullptr,
    //     ImGuiWindowFlags_NoTitleBar |
    //     ImGuiWindowFlags_NoResize |
    //     ImGuiWindowFlags_NoMove |
    //     ImGuiWindowFlags_NoCollapse |
    //     ImGuiWindowFlags_NoScrollbar |
    //     ImGuiWindowFlags_NoBackground);

    // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));
    // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));

    // static int selectedSlot = 0;
    // const char* abilities[] = { "None", "Invisibility", "Speed", "Trap", "Scan" };
    // int slotCount = IM_ARRAYSIZE(abilities);

    // for (int i = 0; i < slotCount; ++i) {
    //     if (i > 0) ImGui::SameLine();

    //     if (i == selectedSlot) {
    //         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.7f, 0.3f, 1.0f)); // highlight
    //     } else {
    //         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    //     }

    //     std::string label = "[" + std::string(abilities[i]) + "]";
    //     if (ImGui::Button(label.c_str(), ImVec2(64, 64))) {
    //         selectedSlot = i;
    //         Window::currentAbility = static_cast<AbilityType>(i); // your own enum
    //     }

    //     ImGui::PopStyleColor();
    // }

    // ImGui::PopStyleVar(2);
    // ImGui::End();

    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Current Game State: %s", "Playing");
    ImGui::End();

    ImGui::Begin("MINIGAME PLACEHOLDER", nullptr, ImGuiWindowFlags_NoResize);
    
    if (ImGui::Button("I DIED", ImVec2(120, 40))) {
        std::cout << "I DIED clicked on" << std::endl;
        Window::currentState = IN_MINIGAME;
        glfwPostEmptyEvent(); // Force frame refresh
    }
    ImGui::End();
}