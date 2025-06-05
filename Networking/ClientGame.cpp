
#include "ClientGame.h" 
// #include <direct.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

ClientGame::ClientGame()
{
    //std::cout << "Starting process of creating ClientGame\n";
    network = new ClientNetwork();
    //std::cout << "ClientNetwork created\n";
    
    window = Window::createWindow(800, 600);
    if (!window) exit(EXIT_FAILURE);
    //std::cout << "Window created\n";

    client_logic::setup_callbacks(window);
    // Setup OpenGL settings.
    client_logic::setup_opengl_settings();

    // Initialize the shader program; exit if initialization fails.
    if (!Window::initializeProgram()) exit(EXIT_FAILURE);
    // Initialize objects/pointers for rendering; exit if initialization fails.
    if (!Window::initializeObjects()) exit(EXIT_FAILURE);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    client_logic::io = &io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("..\\external\\style\\fonts\\Junicode-Bold.ttf", 32.0f);
    #ifdef _WIN32
    io.Fonts->AddFontFromFileTTF("../../external/style/fonts/Junicode-Bold.ttf", 32.0f);
    client_logic::handwritingFont = io.Fonts->AddFontFromFileTTF("../../external/style/fonts/HomemadeApple-Regular.ttf", 28.0f);
    #else 
    io.Fonts->AddFontFromFileTTF("../external/style/fonts/Junicode-Bold.ttf", 32.0f);
    client_logic::handwritingFont = io.Fonts->AddFontFromFileTTF("../external/style/fonts/HomemadeApple-Regular.ttf", 28.0f);
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
    bool ret = client_logic::LoadTextureFromFile("../../external/images/HeistAtTheMuseumTitle.png", &my_image_texture, &my_image_width, &my_image_height);
    #else
    bool ret = client_logic::LoadTextureFromFile("../external/images/HeistAtTheMuseumTitle.png", &my_image_texture, &my_image_width, &my_image_height);
    #endif
    IM_ASSERT(ret);

    // send init packet
    //std::cout << "Sending Init Packet\n";
    sendInitPacket();
    //std::cout << "Init Packet sent\n";
}

void ClientGame::sendPacket(Packet& packet) {
    const unsigned int packet_size = packet.getSize();

    std::vector<char> packet_data(packet_size);  
    packet.serialize(packet_data.data());
    // printf("Packet of size %d\n", packet_size);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data.data(), packet_size);
}

void ClientGame::sendMinigameFinishedPacket() {
    MinigameFinishedPacket packet;
    packet.packet_type = MINIGAME_FINISHED;
    packet.clientID = Window::client_id;
    sendPacket(packet);
}

void ClientGame::sendInitPacket() {
    InitPacket packet;
    packet.packet_type = INIT_CONNECTION;
    packet.character = NONE;
    packet.windowWidth = Window::width;
    packet.windowHeight = Window::height;
    sendPacket(packet);
}

void ClientGame::sendKeyPacket(KeyType key) {
    KeyPacket packet;
    packet.packet_type = KEY_INPUT;
    packet.key_type = key;
    sendPacket(packet);
}

void ClientGame::sendPendingPackets() {
    for (auto& packet : client_logic::pendingPackets) {
        const unsigned int packet_size = packet->getSize();
        std::vector<char> packet_data(packet_size);  
        packet->serialize(packet_data.data());
        NetworkServices::sendMessage(network->ConnectSocket, packet_data.data(), packet_size);
    }
    client_logic::pendingPackets.clear();
}

void ClientGame::update()
{
    glfwPollEvents();

    if (Window::currentState == START_MENU || Window::currentState == CHARACTER_SELECTION) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    if (Window::currentState == START_MENU || Window::currentState == INIT) {
        client_logic::setStartPage(Window::currentState);
    } else if (Window::currentState == CHARACTER_SELECTION || Window::currentState == WAITING){
        client_logic::setCharacterSelectPage(Window::currentState);
    } else if (Window::currentState == PLAYING) {
        client_logic::handleUserInput(window);
        // static bool texturesLoaded = false;
        // if (!texturesLoaded) {
        //     #ifdef _WIN32
        //     const char* base = "../../external/images/";
        //     #else
        //     const char* base = "../external/images/";
        //     #endif
            
        //     client_logic::LoadTextureFromFile((std::string(base) + "thief_win.png").c_str(), 
        //         &client_logic::img_thief_win, &client_logic::img_width, &client_logic::img_height);
        //     client_logic::LoadTextureFromFile((std::string(base) + "thief_lose.png").c_str(), 
        //         &client_logic::img_thief_lose, &client_logic::img_width, &client_logic::img_height);
        //     client_logic::LoadTextureFromFile((std::string(base) + "guard_win.png").c_str(), 
        //         &client_logic::img_guard_win, &client_logic::img_width, &client_logic::img_height);
        //     client_logic::LoadTextureFromFile((std::string(base) + "guard_lose.png").c_str(), 
        //         &client_logic::img_guard_lose, &client_logic::img_width, &client_logic::img_height);

        //     texturesLoaded = true;
        // }

        client_logic::setMainGameWindow(window);
    } else if (Window::currentState == IN_MINIGAME) {
        if (!client_logic::miniGameInitialized) {
            client_logic::miniGame.init(window, client_logic::miniGamePlatformsServer);
            client_logic::miniGameInitialized = true;
            std::cout << "minigame initialized" << std::endl;
        }
        client_logic::miniGame.updateMinigamePlatforms();
        client_logic::miniGame.update(window);
        client_logic::miniGame.render();

        if (client_logic::miniGame.isFinished()) {
            std::cout << "minigame finished" << std::endl;
            client_logic::miniGame.cleanup();
            Window::currentState = PLAYING; 
            client_logic::miniGameInitialized = false;
            sendMinigameFinishedPacket();
        }
    }
    
    sendPendingPackets();

    while (true) {
        int headerSize = Packet::getHeaderSize();
        std::vector<char> header(headerSize);
        int data_length = network->receivePackets(header.data(), headerSize);

        if (data_length == -1) {
            break;
        }
        Packet temp;
        temp.deserializeHeader(header.data());
        //char data[packet.length];
        std::vector<char> full_packet(headerSize + temp.length);
        std::copy(header.begin(), header.end(), full_packet.begin());
        data_length = network->receivePackets(full_packet.data() + headerSize, temp.length);
        std::unique_ptr<Packet> packet = PacketFactory::createFromBuffer(full_packet.data());

        switch (temp.packet_type) {
            case INIT_PLAYER: {
                InitPlayerPacket* initPlayerPacket = dynamic_cast<InitPlayerPacket*>(packet.get());
                // TODO: Make this work with packet class
                printf("recieved init player packet from server\n");
                /*Window::setClientID(packet.payload.data());*/
                Window::setInitState(*initPlayerPacket);
                break;
            }
            case GUI_UPDATE: {
                // printf("recieved gui update packet from server\n");
                GuiUpdatePacket* guiPacket = dynamic_cast<GuiUpdatePacket*>(packet.get());
                Window::applyGuiUpdate(*guiPacket);
                client_logic::updateAvailableChars(*guiPacket);
                // Window::currentState = guiPacket->currentState;
                break;
            }
            case ECHO_EVENT:
                printf("client recieved echo event packet from server\n");
                break;
            case STATE_UPDATE: {
                // printf("client recieved state update from server\n");
                StateUpdatePacket* statePacket = dynamic_cast<StateUpdatePacket*>(packet.get());
                // printf("payload size: %d\n", packet.length);
                // std::cout << "Window init: " << Window::initialized << std::endl;
                if (Window::initialized) {
                    // printf("processinfg\n");
                    Window::applyServerState(*statePacket);
                }
                // Window::render(window);
                // Window::cube->update();
                break;
            }
            case END_GAME: {
                printf("client end game packet from server\n");
                EndGamePacket* endPacket = dynamic_cast<EndGamePacket*>(packet.get());
                unsigned int closedClient = endPacket->closedClient;

                if (closedClient == Window::client_id) {
                    #ifdef _WIN32
                    closesocket(network->ConnectSocket);
                    #else
                    close(network->ConnectSocket);
                    #endif
                    glfwSetWindowShouldClose(window, GL_TRUE);
                } else {
                    Window::removeClient(closedClient);
                }
                break;
            }
            case WIN_STATE: {
                WinPacket* winPacket = dynamic_cast<WinPacket*>(packet.get());
                if (winPacket) {
                    if (winPacket->winnerType == WinnerType::GUARD) {
                        std::cout << "Security Guard Wins!" << std::endl;
                    } else {
                        std::cout << "Thief Wins! Winner ID: " << winPacket->winningClientID << std::endl;
                    }

                    Window::currentState = WIN_CONDITION;
                }
                // client_logic::gameResult = (winPacket->winningClientID == Window::client_id)
                //                             ? ((client_logic::playerRole == CharacterType::CHARACTER_4) 
                //                                 ? WinState::GUARD_WIN 
                //                                 : WinState::THIEF_WIN)
                //                             : ((client_logic::playerRole == CharacterType::CHARACTER_4) 
                //                                 ? WinState::THIEF_WIN 
                //                                 : WinState::GUARD_WIN);
                if (winPacket->winnerType == WinnerType::THIEF) {
                    client_logic::gameResult = WinState::THIEF_WIN;
                } else {
                    client_logic::gameResult = WinState::GUARD_WIN;
                }

                std::cout << "[Client] Received WIN_STATE, gameResult = " << (int)client_logic::gameResult << std::endl;

                // printf("Client %u wins the game!\n", winPacket->winningClientID);
                break;
            }
            case TIME_UPDATE: {
                TimeUpdate* timePacket = dynamic_cast<TimeUpdate*>(packet.get());
                // std::cout << "yes in case time_update "<< std::endl;
                if (timePacket) {
                    // std::cout << "yes inside if timePacket  "<< std::endl;
                    client_logic::currentTimeString = timePacket->timeString;
                    // std::cout << "current time : in case time_update: " << client_logic::currentTimeString << std::endl;
                }
                break;
            }
            /* case MINIGAME_SETUP: {
                MinigamePacket* packet = dynamic_cast<MinigamePacket*>(packet.get());

                if (Window::currentState != IN_MINIGAME) {
                    Window::currentState = IN_MINIGAME;
                    Window::miniGame.init(window, packet->platforms);
                } */
            // }
            case INIT_MINIGAME: {
                InitMinigamePacket* initMinigamePacket = dynamic_cast<InitMinigamePacket*>(packet.get());
                printf("recieved init minigame packet from server\n");
                client_logic::setMinigamePlatformsServer(*initMinigamePacket);
                break;
            }

            default:
                printf("client received unknown packet type from server\n");
                break;
        }
    }
//   printf("curr state: %d\n", Window::currentState);
    if (Window::currentState != IN_MINIGAME){
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    } /* else if (Window::currentState == IN_MINIGAME) {
        Window::miniGame.render();
    } */
    
    // Window::render(window);
    glfwSwapBuffers(window);
    // Window::updateRest(packet.payload.data());
    // Window::applyRest(packet.payload.data(), , packet.length);
    // Window::cube->update();
    // Window::idleCallback();
}

