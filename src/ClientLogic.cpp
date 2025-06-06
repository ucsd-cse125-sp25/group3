#include "ClientLogic.h" 

WinState client_logic::gameResult = WinState::NONE;
CharacterType client_logic::playerRole = CharacterType::NONE;

// GLuint client_logic::img_thief_win = 0;
// GLuint client_logic::img_thief_lose = 0;
// GLuint client_logic::img_guard_win = 0;
// GLuint client_logic::img_guard_lose = 0;
// int client_logic::img_width = 0;
// int client_logic::img_height = 0;

ImVec2 client_logic::displaySize;
ImGuiIO* client_logic::io;
ImFont* client_logic::handwritingFont;
bool client_logic::availableChars[4];
std::vector<std::unique_ptr<Packet>> client_logic::pendingPackets;

// timer 
std::string client_logic::currentTimeString = "05:00";
 ImFont* client_logic::s_font_italic ;
 ImFont* client_logic::s_font_bold ;
GLuint client_logic::background_texture;
int client_logic::background_width, client_logic::background_height;
GLuint client_logic::title_texture;
int client_logic::title_width = 0, client_logic::title_height = 0;
bool client_logic::audio_enabled = true;


void client_logic::updateAvailableChars(GuiUpdatePacket& packet) {

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

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        key = KeyType::KEY_C;

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
    // stbi_set_flip_vertically_on_load(true);

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




static std::map<std::string, float> hoverTime;

void client_logic::RenderFancyTextButton(const char* label, bool& clicked, ImFont* fontNormal, ImFont* fontHover, bool& selected, AudioManager* audio) {
    std::string labelKey(label);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 textSize = ImGui::CalcTextSize(label);

    float cursorX = (ImGui::GetWindowSize().x - textSize.x) * 0.5f;
    ImGui::SetCursorPosX(cursorX);
    pos = ImGui::GetCursorScreenPos();

    ImGui::InvisibleButton(label, textSize);
    bool hovered = ImGui::IsItemHovered();
    clicked = ImGui::IsItemClicked();
    if (clicked && client_logic::audio_enabled) {
        audio->playSound("button_click");
    }

    // Hover pulse animation
    if (hovered) {
        hoverTime[labelKey] += ImGui::GetIO().DeltaTime;
    } else {
        hoverTime[labelKey] = 0.0f;
    }

    float t = hoverTime[labelKey];
    float pulse = hovered ? (0.6f + 0.4f * (std::sin(t * 4.0f) + 1.0f) * 0.5f) : 1.0f;

    ImFont* font = hovered ? fontHover : fontNormal;
    ImGui::PushFont(font);

    ImU32 baseTextColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.627f, 0.478f, 0.333f, 1.0f));
    ImU32 glowTextColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.9f, 0.5f, 1.0f));
    ImU32 textColor = hovered ? glowTextColor : baseTextColor;


    if (selected) {
        textColor = ImGui::ColorConvertFloat4ToU32(ImVec4(80/255.f, 46/255.f, 9/255.f, 1.0f));
        font = fontHover;
        // std::cout << "selected" << std::endl;
    }

    if (hovered) {
        for (int i = 1; i <= 3; ++i) {
            float offset = i * pulse;
            ImU32 glowColor = IM_COL32(255, 220, 100, static_cast<int>(40 / i));
            drawList->AddText(ImVec2(pos.x + offset, pos.y + offset), glowColor, label);
            drawList->AddText(ImVec2(pos.x - offset, pos.y + offset), glowColor, label);
            drawList->AddText(ImVec2(pos.x + offset, pos.y - offset), glowColor, label);
            drawList->AddText(ImVec2(pos.x - offset, pos.y - offset), glowColor, label);
        }
    }

    drawList->AddText(pos, textColor, label);
    ImGui::PopFont();
}

void client_logic::RenderFancyTextButton(const char* label, bool& clicked, ImFont* fontNormal, ImFont* fontHover) {
    std::string labelKey(label);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 textSize = ImGui::CalcTextSize(label);

    float cursorX = (ImGui::GetWindowSize().x - textSize.x) * 0.5f;
    ImGui::SetCursorPosX(cursorX);
    pos = ImGui::GetCursorScreenPos();

    ImGui::InvisibleButton(label, textSize);
    bool hovered = ImGui::IsItemHovered();
    clicked = ImGui::IsItemClicked();

    // Hover pulse animation
    if (hovered) {
        hoverTime[labelKey] += ImGui::GetIO().DeltaTime;
    } else {
        hoverTime[labelKey] = 0.0f;
    }

    float t = hoverTime[labelKey];
    float pulse = hovered ? (0.6f + 0.4f * (std::sin(t * 4.0f) + 1.0f) * 0.5f) : 1.0f;

    ImFont* font = hovered ? fontHover : fontNormal;
    ImGui::PushFont(font);
    ImU32 baseTextColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.627f, 0.478f, 0.333f, 1.0f));
    ImU32 glowTextColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.9f, 0.5f, 1.0f));
    ImU32 textColor = hovered ? glowTextColor : baseTextColor;

    if (hovered) {
        for (int i = 1; i <= 3; ++i) {
            float offset = i * pulse;
            ImU32 glowColor = IM_COL32(255, 220, 100, static_cast<int>(40 / i));
            drawList->AddText(ImVec2(pos.x + offset, pos.y + offset), glowColor, label);
            drawList->AddText(ImVec2(pos.x - offset, pos.y + offset), glowColor, label);
            drawList->AddText(ImVec2(pos.x + offset, pos.y - offset), glowColor, label);
            drawList->AddText(ImVec2(pos.x - offset, pos.y - offset), glowColor, label);
        }
    }

    drawList->AddText(pos, textColor, label);
    ImGui::PopFont();
}



void client_logic::setStartPage(GameState currState) {
    // TODO: Make load textures and fonts somewhere else (outside of the while loop)
    

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    if (currState == INIT) return;


    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 window_size = viewport->Size;

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(window_size);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Start Menu Background", nullptr, flags);

    // Draw background image
    if (client_logic::background_texture) {
        ImGui::GetWindowDrawList()->AddImage(
            client_logic::background_texture,
            viewport->Pos,
            ImVec2(viewport->Pos.x + window_size.x, viewport->Pos.y + window_size.y),
            ImVec2(0, 0), ImVec2(1, 1)
        );
    }


    float title_scale = (window_size.x * 0.5f) / 823.0f;
    ImVec2 title_size(823.0f * title_scale, 463.0f * title_scale);

    float spacing = window_size.y * 0.04f;
    float button_height = ImGui::CalcTextSize("Start").y;  // assuming 1-line height
    int num_buttons = 3;

    float total_content_height = title_size.y + spacing + num_buttons * (button_height + spacing);

    // Center the whole block vertically
    float content_top = (window_size.y - total_content_height) * 0.5f + spacing * 0.5f;
    ImGui::SetCursorPosY(content_top);
    // Title image (centered horizontally)
    ImVec2 title_pos = ImVec2((window_size.x - title_size.x) * 0.5f, ImGui::GetCursorPosY());
    ImGui::SetCursorPos(title_pos);
    ImGui::Image(client_logic::title_texture, title_size);

    // Add vertical spacing before buttons
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + spacing);

    // Draw each button, centered
    bool clicked = false;

    RenderFancyTextButton("Start", clicked, client_logic::s_font_italic, client_logic::s_font_bold);
    if (clicked) {
        auto packet = std::make_unique<KeyPacket>();
        packet->packet_type = KEY_INPUT;
        packet->key_type = KeyType::MENU_START;
        pendingPackets.push_back(std::move(packet));
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + spacing);
    RenderFancyTextButton(audio_enabled ? "Audio: On" : "Audio: Off", clicked, client_logic::s_font_italic, client_logic::s_font_bold);
    if (clicked) audio_enabled = !audio_enabled;

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + spacing);
    RenderFancyTextButton("Quit", clicked, client_logic::s_font_italic, client_logic::s_font_bold);
    if (clicked) glfwSetWindowShouldClose(glfwGetCurrentContext(), true);

    ImGui::End();
    ImGui::PopStyleVar();

    // // Mandatory ImGui frame termination
    // ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    // ImGui::Text("Current Game State: %s", "START MENU");
    // ImGui::End();
}

void client_logic::setCharacterSelectPage(GameState currState, GLFWwindow* window, AudioManager* audio) {

    ImVec2 windowSize = io->DisplaySize;
    
    
    GLuint parchment_texture;
    int parchmentWidth, parchmentHeight;
    #ifdef _WIN32
    bool ok = LoadTextureFromFile("../../external/images/parchment_scroll.png", &parchment_texture, &parchmentWidth, &parchmentHeight);
    #else 
    bool ok = LoadTextureFromFile("../external/images/parchment_scroll.png", &parchment_texture, &parchmentWidth, &parchmentHeight);
    #endif
    IM_ASSERT(ok);
   
    


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    Window::renderCharacterPreview(window);

    if (!window) {
        std::cerr << "Error: Window is not initialized!" << std::endl;
        return;
    }

    // Window::render(window);
    static bool showInfo[4] = { false, false, false, false };
    

    const char* names[] = { "Geoffreybella", "Anthonyia", "Curtina", "Voelkernessa" };
    const char* abilities[] = {
        "Cloakfade  Become invisible for 3 seconds.",
        "Ghoststep Double speed for 3 seconds.",
        "EchoVision See all player positions on map for 5 seconds.",
        "OmniScan Periodically reveals all player positions."
    };
    const char* bios[] = {
        "Geoffreybella grew up in foggy London, mastering illusions as a magician’s assistant. She now uses her tricks to disappear mid-heist.",
        "Anthonyia, raised in Marseille, was once a getaway driver. Now she races past cameras and guards for the ultimate adrenaline rush.",
        "Curtina, a former hacker prodigy, watches from the shadows, guiding teammates through laser fields with her surveillance skills.",
        "Voelkernessa, a retired military strategist, turned the museum into her fortress. She knows its secrets better than its curators."
    };


    


    // Define style and background
    float panelWidth = windowSize.x * .30f;
    float panelHeight = windowSize.y * 0.80f;

    ImVec2 centerPos = ImVec2((windowSize.x - panelWidth) * 0.5f, (windowSize.y - panelHeight) * 0.5f);
    ImVec2 leftCenteredPos = ImVec2(
        windowSize.x * 0.1f,                                  // 10% from left edge
        (windowSize.y - panelHeight) * 0.5f                   // vertically centered
    );
    ImGui::SetNextWindowPos(leftCenteredPos);

    ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));

    ImGui::Begin("Parchment Panel", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground
    );


    if (parchment_texture) {
        ImGui::GetWindowDrawList()->AddImage(
            parchment_texture,
            ImGui::GetWindowPos(),
            ImVec2(ImGui::GetWindowPos().x + panelWidth, ImGui::GetWindowPos().y + panelHeight),
            ImVec2(0, 0), ImVec2(1, 1));
    }


    // Apply warm vintage style
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.50f, 0.38f, 0.24f, 1.0f));  // tan
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.55f, 0.30f, 1.0f));  // lighter brown
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f, 0.45f, 0.25f, 1.0f));  // deeper brown
    ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(0.98f, 0.92f, 0.84f, 1.0f));  // parchment cream

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16, 14));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    // Optional: Vintage font
    if (s_font_italic) ImGui::PushFont(s_font_italic);

    // Character selection
    bool clicked = false;
    bool isSelected = true;

    static int selectedSlot = -1;
    const char* roles[] = { "Geoffreybella", "Anthonyia", "Curtina", "Voelkernessa" };

    // abilities and info (birthdays?)

    int roleCount = IM_ARRAYSIZE(roles);

    if (selectedSlot != -1 && !availableChars[selectedSlot]) {
        selectedSlot = -1;
    }

    static CharacterType selCharacter;
    ImGui::Dummy(ImVec2(0.0f, 120.0f));  // smaller vertical space

    client_logic::RenderFancyTextButton("Select Your Character", clicked, s_font_italic, s_font_bold, isSelected, audio);
    ImGui::Dummy(ImVec2(0.0f, 50.0f));  // smaller vertical space
    if (clicked ) {
            selectedSlot = -1;
    }


    for (int i = 0; i < roleCount; ++i) {
        if (i > 0) ImGui::Dummy(ImVec2(0.0f, 20.0f));

        clicked = false;
        isSelected = (selectedSlot == i);
        std::string label = roles[i];

        if (!availableChars[i]) ImGui::BeginDisabled();

        client_logic::RenderFancyTextButton(label.c_str(), clicked, s_font_italic, s_font_bold, isSelected, audio);

        if (clicked && availableChars[i]) {
            selectedSlot = i;
            selCharacter = static_cast<CharacterType>(i);
            playerRole = selCharacter;// your own enum
        }

        if (!availableChars[i]) ImGui::EndDisabled();
    }
    Window::setCharacterPreview(selCharacter);
    



    ImGui::Spacing();
    ImGui::Spacing();

    // Options
    ImVec2 buttonSize(400, 64);
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float totalWidth = buttonSize.x * 2 + spacing;
    float offsetX = (ImGui::GetWindowSize().x - totalWidth) * 0.5f;
    ImGui::SetCursorPosX(offsetX);

    float paddingX = 40.0f; // Add horizontal padding
    float paddingY = 20.0f; // Add vertical padding

    ImVec2 backSize = ImGui::CalcTextSize("Back");
    backSize.x += paddingX;
    backSize.y += paddingY;

    ImVec2 randSize = ImGui::CalcTextSize("Randomize");
    randSize.x += paddingX;
    randSize.y += paddingY;

    spacing = ImGui::GetStyle().ItemSpacing.x;
    totalWidth = backSize.x + randSize.x + spacing;
    offsetX = (ImGui::GetWindowSize().x - totalWidth) * 0.5f;
    ImGui::SetCursorPosX(offsetX);

    if (ImGui::Button("Back", backSize)) {
        auto packet = std::make_unique<KeyPacket>();
        packet->packet_type = KEY_INPUT;
        packet->key_type = KeyType::CHAR_SEL_BACK;
        pendingPackets.push_back(std::move(packet));
    }

    ImGui::SameLine();

    if (ImGui::Button("Randomize", randSize)) {
        // Step 1: Gather indices of available characters
        std::vector<int> availableIndices;
        for (int i = 0; i < IM_ARRAYSIZE(roles); ++i) {
            if (availableChars[i]) {
                availableIndices.push_back(i);
            }
        }

        // Step 2: Pick a random one
        if (!availableIndices.empty()) {
            int randomIndex = availableIndices[rand() % availableIndices.size()];
            selectedSlot = randomIndex;
            selCharacter = static_cast<CharacterType>(randomIndex);
        }
    }
    ImVec2 confirmSize = ImGui::CalcTextSize("Confirm");
    confirmSize.x += paddingX;
    confirmSize.y += paddingY;

    ImVec2 storySize = ImGui::CalcTextSize("Story");
    storySize.x += paddingX;
    storySize.y += paddingY;

    spacing = ImGui::GetStyle().ItemSpacing.x;
    float totalConfirmRowWidth = confirmSize.x + storySize.x + spacing;
    float rowOffsetX = (ImGui::GetWindowSize().x - totalConfirmRowWidth) * 0.5f;
    ImGui::SetCursorPosX(rowOffsetX);

    ImGui::BeginDisabled(selectedSlot == -1);
    if (ImGui::Button("Confirm", confirmSize)) {
        auto packet = std::make_unique<InitPacket>();
        packet->packet_type = INIT_CONNECTION;
        packet->character = selCharacter;
        packet->windowWidth = Window::width;
        packet->windowHeight = Window::height;
        pendingPackets.push_back(std::move(packet));
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    if (ImGui::Button(("Story##" + std::to_string(selectedSlot)).c_str(), storySize)) {
        showInfo[selectedSlot] = true;
    }


    if (s_font_italic) ImGui::PopFont();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(4);
    ImGui::End();

    // Right-side panel for 3D model or placeholder
    ImVec2 modelPanelSize = ImVec2(windowSize.x * 0.5f, panelHeight);
    ImVec2 modelPanelPos = ImVec2(windowSize.x * 0.5f + windowSize.x * 0.1f, (windowSize.y - panelHeight) * 0.5f);

    ImGui::SetNextWindowPos(modelPanelPos);
    ImGui::SetNextWindowSize(modelPanelSize);

    ImGui::Begin("CharacterModelPreview", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground);

    ImGui::SetCursorPosY(modelPanelSize.y * 0.45f);
ImGui::SetCursorPosX((modelPanelSize.x - ImGui::CalcTextSize("How to Play").x) * 0.5f);
ImGui::Text("How to Play");

ImGui::Dummy(ImVec2(0.0f, 12.0f));
ImGui::SetCursorPosX(modelPanelSize.x * 0.15f);
ImGui::Text("- Use WASD or arrow keys to move");
ImGui::SetCursorPosX(modelPanelSize.x * 0.15f);
ImGui::Text("- Use mouse to adjust camera");
ImGui::SetCursorPosX(modelPanelSize.x * 0.15f);
ImGui::Text("- Press E to use ability");
ImGui::SetCursorPosX(modelPanelSize.x * 0.15f);
ImGui::Text("- Press F to pick up items");
ImGui::SetCursorPosX(modelPanelSize.x * 0.15f);
ImGui::Text("- Press C to catch the thief");


    ImGui::End();
    // ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    // ImGui::Text("Current Game State: %s", "START MENU");
    // ImGui::End();


    for (int i = 0; i < 4; ++i) {
        if (showInfo[i]) {
            ImGui::OpenPopup(("Bio##" + std::to_string(i)).c_str());
        }
        ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight));
ImGui::SetNextWindowPos(leftCenteredPos);

if (ImGui::BeginPopupModal(("Bio##" + std::to_string(i)).c_str(), &showInfo[i],
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {


    // Overall styling
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));  // black background
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));      // default text color (light gray)
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(80 / 255.f, 46 / 255.f, 9 / 255.f, 1.0f));  // dark brown
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1550, 0));  // wider

    // Header (name) with tan color
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.627f, 0.478f, 0.333f, 1.0f));  // tan
    ImGui::Text("%s", names[i]);
    ImGui::PopStyleColor();

    ImGui::Separator();
    ImGui::TextWrapped("Ability: %s", abilities[i]);
    ImGui::Spacing();
    ImGui::TextWrapped("%s", bios[i]);

    ImGui::Spacing();
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 100) * 0.5f);  // Center button
    if (ImGui::Button("Close", ImVec2(100, 40))) {
        showInfo[i] = false;
        ImGui::CloseCurrentPopup();
    }

    // Cleanup
    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(3);

    ImGui::EndPopup();
    }
    
    }




}

void client_logic::setMainGameWindow(GLFWwindow* window) {
    ImVec2 windowSize = io->DisplaySize;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    Window::render(window);

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 200, ImGui::GetIO().DisplaySize.y - 100));
    ImGui::SetNextWindowSize(ImVec2(400, 80));


    // ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    // ImGui::Text("Current Game State: %s", "Playing");
    // ImGui::End();

    // ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    // ImGui::Text("Current Game State: %s", "Playing");
    // ImGui::End();

    // ImGui::Begin("MINIGAME PLACEHOLDER", nullptr, ImGuiWindowFlags_NoResize);
    
    // if (ImGui::Button("I DIED", ImVec2(120, 40))) {
    //     // currentState = IN_MINIGAME;
    //     // stateChanged = true; // Flag for state transition
    //     // glfwPostEmptyEvent(); // Force frame refresh
    // }
    // ImGui::End();

    // // Show test countdown timer "Time: 04:59" at top center
    ImVec2 screenSize = io->DisplaySize;
    ImVec2 windowSize1 = ImVec2(200, 50);
    ImVec2 windowPos = ImVec2((screenSize.x - windowSize1.x) * 0.5f, 20.0f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize1);
    ImGui::Begin("##TimeTest", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);

    // const char* timeText = "Time: 04:59";
    // float textWidth = ImGui::CalcTextSize(timeText).x;
    // ImGui::SetCursorPosX((windowSize1.x - textWidth) * 0.5f);
    // ImGui::Text("%s", timeText);
    // std::cout << "in client logic setmain game window :" <<currentTimeString<< std::endl;
    std::string timeDisplay = "Time: " + currentTimeString;
    float textWidth = ImGui::CalcTextSize(timeDisplay.c_str()).x;
    ImGui::SetCursorPosX((windowSize1.x - textWidth) * 0.5f);
    ImGui::Text("%s", timeDisplay.c_str());


    ImGui::End();
    std::cout << (int)gameResult << std::endl;

    if (gameResult != WinState::NONE) {

        GLuint resultTexture_theif = 0;
        GLuint resultTexture_guard = 0;
        int resultImageWidth = 0;
        int resultImageHeight = 0;
        //static bool texturesLoaded = false;
        /*static bool resultTextureLoaded_thief = false;
        static bool resultTextureLoaded_guard = false;*/

        if((int)gameResult == 0)//theives win
        {
            
            /*if(!texturesLoaded)
            {*/
                #ifdef _WIN32
                bool resultTextureLoaded_thief = client_logic::LoadTextureFromFile(
                    
                    "../../external/images/result_pic/thief_win.png",
                    &resultTexture_theif,
                    &resultImageWidth,
                    &resultImageHeight);
                bool resultTextureLoaded_guard = client_logic::LoadTextureFromFile(
                    
                    "../../external/images/result_pic/guard_lose.png",
                    &resultTexture_guard,
                    &resultImageWidth,
                    &resultImageHeight);    
                //texturesLoaded = true;
                #else 
                bool resultTextureLoaded_thief = client_logic::LoadTextureFromFile(                
                    "../external/images/result_pic/thief_win.png",
                    &resultTexture_theif,
                    &resultImageWidth,
                    &resultImageHeight);
                bool resultTextureLoaded_guard = client_logic::LoadTextureFromFile(                
                    "../external/images/result_pic/guard_lose.png",
                    &resultTexture_guard,
                    &resultImageWidth,
                    &resultImageHeight);
                //texturesLoaded = true;
                #endif
            //}
            
            if(resultTextureLoaded_thief && resultTextureLoaded_guard)
            {

                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::SetNextWindowSize(io->DisplaySize);

                ImGui::Begin("Game End Screen", nullptr,
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoBackground);

                float maxDisplayWidth = io->DisplaySize.x;
                float maxDisplayHeight = io->DisplaySize.y;
                float scale = (std::max)(maxDisplayWidth / resultImageWidth, maxDisplayHeight / resultImageHeight);
                float displayWidth = resultImageWidth * scale;
                float displayHeight = resultImageHeight * scale;

                // float posX = (io->DisplaySize.x - displayWidth) * 0.5f;
                // float posY = (io->DisplaySize.y - displayHeight) * 0.5f;
                ImGui::SetCursorPos(ImVec2(0, 0));
                std::cout << "characterrrrrr type :  " << playerRole << std::endl;
                 //std::cout << "characterrrrrr type :  " << client_logic::playerRole << std::endl;
                if (playerRole == 3)
                {
                    std::cout << "security guard lose "<< std::endl;
                    ImGui::Image((ImTextureID)(intptr_t)resultTexture_guard, ImVec2(displayWidth, displayHeight));
                }
                else
                {
                    ImGui::Image((ImTextureID)(intptr_t)resultTexture_theif, ImVec2(displayWidth, displayHeight));
                }

                ImGui::Text("Game End!!!!!");
                ImGui::End();
            }

        }
        else if((int)gameResult == 1)//security guard win
        {
            // std::cout << "yes inside game result 11  "<< std::endl;
             #ifdef _WIN32
                bool resultTextureLoaded_thief = client_logic::LoadTextureFromFile(
                    
                    "../../external/images/result_pic/thief_lose.png",
                    &resultTexture_theif,
                    &resultImageWidth,
                    &resultImageHeight);
                bool resultTextureLoaded_guard = client_logic::LoadTextureFromFile(
                    
                    "../../external/images/result_pic/guard_win.png",
                    &resultTexture_guard,
                    &resultImageWidth,
                    &resultImageHeight);    
                //texturesLoaded = true;
                #else 
                bool resultTextureLoaded_thief = client_logic::LoadTextureFromFile(                
                    "../external/images/result_pic/thief_lose.png",
                    &resultTexture_theif,
                    &resultImageWidth,
                    &resultImageHeight);
                bool resultTextureLoaded_guard = client_logic::LoadTextureFromFile(                
                    "../external/images/result_pic/guard_win.png",
                    &resultTexture_guard,
                    &resultImageWidth,
                    &resultImageHeight);
                //texturesLoaded = true;
                #endif

                if(resultTextureLoaded_thief && resultTextureLoaded_guard)
                {

                    ImGui::SetNextWindowPos(ImVec2(0, 0));
                    ImGui::SetNextWindowSize(io->DisplaySize);

                    ImGui::Begin("Game End Screen", nullptr,
                        ImGuiWindowFlags_NoTitleBar |
                        ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoBringToFrontOnFocus |
                        ImGuiWindowFlags_NoBackground);

                    float maxDisplayWidth = io->DisplaySize.x;
                    float maxDisplayHeight = io->DisplaySize.y;
                    float scale = (std::max)(maxDisplayWidth / resultImageWidth, maxDisplayHeight / resultImageHeight);
                    float displayWidth = resultImageWidth * scale;
                    float displayHeight = resultImageHeight * scale;

                    // float posX = (io->DisplaySize.x - displayWidth) * 0.5f;
                    // float posY = (io->DisplaySize.y - displayHeight) * 0.5f;
                    ImGui::SetCursorPos(ImVec2(0, 0));
                    std::cout << "characterrrrrr type :  " << playerRole << std::endl;
                    if (playerRole == 3)
                    {
                        std::cout << "security guard win "<< std::endl;
                        ImGui::Image((ImTextureID)(intptr_t)resultTexture_guard, ImVec2(displayWidth, displayHeight));
                    }
                    else
                    {
                        ImGui::Image((ImTextureID)(intptr_t)resultTexture_theif, ImVec2(displayWidth, displayHeight));
                    }

                    ImGui::Text("Game End!!!!!");
                    ImGui::End();
                }


        }
       

        

    }


}

