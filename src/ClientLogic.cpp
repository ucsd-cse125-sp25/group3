#include "ClientLogic.h" 

ImFont* client_logic::handwritingFont;
std::vector<std::unique_ptr<Packet>> client_logic::pendingPackets;

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

const char* GameStateToString(GameState state) {
    switch (state) {
        case START_MENU: return "START MENU";
        case CHARACTER_SELECTION: return "CHARACTER SELECTION";
        case PLAYING: return "PLAYING";
        case IN_MINIGAME: return "IN MINIGAME";
        default: return "UNKNOWN";
    }
}

void client_logic::setStartPage() {
    GLuint parchmentTexture;
    int parchmentWidth, parchmentHeight;
    bool ok = LoadTextureFromFile("../external/images/parchment_scroll.png", &parchmentTexture, &parchmentWidth, &parchmentHeight);
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
        // currentState = CHARACTER_SELECTION;
        auto packet = std::make_unique<KeyPacket>();
        packet->packet_type = KEY_INPUT;
        packet->key_type = KeyType::KEY_START;
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