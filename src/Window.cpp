#include "Window.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "AnInstance.h"
// #include <assimp/cimport.h>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
#include <string>

unsigned int Window::client_id;
bool Window::initialized = false;
GameState Window::currentState = INIT;
bool Window::stateChanged = false;

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Cube* Window::cube;
Cube* Window::floor;
Cube* Window::artifact;
std::map<unsigned int, Cube*> Window::cubes;
NPCs* Window::NPC;
std::map<unsigned int, NPCs*> Window::npcs;
Character* Window::character;
std::map<unsigned int, Character*> Window::characters;

Scene* Window::scene;

AnimationPlayer* Window::animationPlayer;

// Camera Properties
Camera* Cam;
Camera* MiniMapCam;

AnInstance* bro2;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// // The shader program id
// GLuint Window::shaderProgram;
// GLuint Window::shaderProgram_uv;
// GLuint Window::shaderProgram_anim;
ShaderManager* Window::shaderManager;
ModelManager* Window::modelManager;

TextureManager* Window::textureManager;

bool Window::altDown = false;
bool Window::firstMouse = true;

// // for show-on-map ability 
bool Window::showOthersOnMiniMap = false;
bool Window::radarActive = false;
std::chrono::steady_clock::time_point Window::radarStartTime;
AbilityType Window::currentAbility = AbilityType::NONE;

// Constructors and desctructors
bool Window::initializeProgram() {
    // // Create a shader program with a vertex shader and a fragment shader.
    // shaderProgram = LoadShaders("../shaders/shader.vert", "../shaders/shader.frag", false);

    // // Check the shader program.
    // if (!shaderProgram) {
    //     std::cerr << "Failed to initialize shader program" << std::endl;
    //     return false;
    // }

    // shaderProgram_uv = LoadShaders("../shaders/shader_uv.vert", "../shaders/shader_uv.frag", true);

    // if (!shaderProgram_uv) {
    //     std::cerr << "Failed to initialize shader program with uvs" << std::endl;
    //     return false;
    // }

    // shaderProgram_anim = LoadShaders("../shaders/anim_shader_multi.vert", "../shaders/anim_shader.frag", true);

    // if (!shaderProgram_uv) {
    //     std::cerr << "Failed to initialize shader program with uvs" << std::endl;
    //     return false;
    // }
    shaderManager = new ShaderManager();

    modelManager = new ModelManager();

    textureManager = new TextureManager();

    animationPlayer = new AnimationPlayer();

    return true;
}

bool Window::initializeObjects() {
    // Create a cube
    cube = new Cube();
    artifact = new Cube(glm::vec3(-0.5, 0, -1), glm::vec3(0, 0.5, 1));
    artifact->setColor(glm::vec3(0.0f, 0.6f, 1.0f));
    artifact->setBaseModel(glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.0f, 2.0f)));
    cube->setCarriedArtifact(artifact);
    floor = new Cube(glm::vec3(-8, -2.03, -8), glm::vec3(8, -2.01, 8));
    NPC = new NPCs(new Cube(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)));


    scene = new Scene();

    //load in all thief related things

    std::map<std::string, AnimState> female_thief_animations = {
        {"Armature.002|Holding Item", AnimState::FT_Hold_Item}, 
        {"Armature.002|Idle", AnimState::FT_Idle},
        {"Armature.002|Looking Around", AnimState::FT_Look_Around},
        {"Armature.002|Picking Up Item", AnimState::FT_Pick_Up},
        {"Armature.002|Running", AnimState::Run},
        {"Armature.002|Walking", AnimState::FT_Walk}
    };

    if (!animationPlayer->loadAnims(ModelType::FemaleThief, female_thief_animations, "../models/characters/feamle_thief.fbx")) {
        std::cout << "animation loading failed" << std::endl;
    }
    Model* femaleThief = new Model(ModelType::FemaleThief, animationPlayer, "../models/characters/feamle_thief.fbx", textureManager);
    femaleThief->setMMat(glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(0.002f)), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    modelManager->addModel(femaleThief);

    // load in all security guard related things

    std::map<std::string, AnimState> security_guard_animations = {
        {"Armature.003|Running.004", AnimState::Run}, 
        {"Armature.003|Shooting Gun", AnimState::SG_Shooting_Gun},
        {"Armature.003|Throw", AnimState::SG_Throw}
    };

    if (!animationPlayer->loadAnims(ModelType::SecurityGuard, security_guard_animations, "../models/characters/security_guard.fbx")) {
        std::cout << "animation loading failed" << std::endl;
    }
    Model* securityGuard = new Model(ModelType::SecurityGuard, animationPlayer, "../models/characters/security_guard.fbx", textureManager);
    securityGuard->setMMat(glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(0.001f)), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    modelManager->addModel(securityGuard);

    // AnInstance* bro = new AnInstance(securityGuard);
    // bro->setState(AnimState::Run);
    // scene->addInstance(bro);

    std::cout << "instance" << std::endl;

    // AnInstance* bro2 = new AnInstance(modelManager->getModel(ModelType::FemaleThief));
    // bro2->setMMat(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f)));
    // bro2->setState(AnimState::Run);
    // scene->addInstance(bro2);

    std::cout << "instance2" << std::endl;

    Model* museum = new Model(ModelType::Museum, "../models/map/museum.fbx", textureManager);
    museum->setMMat(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f)));
    modelManager->addModel(museum);
    AnInstance* museumInstance = new AnInstance(museum);
    scene->addInstance(museumInstance);

    // Model* buddha = new Model(ModelType::Buddha, "../models/buddha.ply", textureManager);
    // // Model* buddha = new Model(ModelType::Museum, "../models/map/museum.fbx", textureManager);
    // modelManager->addModel(buddha);

    AnInstance* model = new AnInstance(modelManager->getModel(ModelType::SecurityGuard));
    //currently no idle state for the security guard
    model->setState(AnimState::Stop);

    character = new Character(model);
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    // floor = new Cube(glm::vec3(-8, -2.03, -8), glm::vec3(8, -2.01, 8));
    
    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    printf("deleting\n");
    delete cube;
    delete artifact;
    delete floor;
    delete Cam;
    delete MiniMapCam;
    delete scene;
    delete character;
    
    delete textureManager;
    delete animationPlayer;
    // Delete the shader program.
    // glDeleteProgram(shaderProgram);
    // glDeleteProgram(shaderProgram_uv);
    // glDeleteProgram(shaderProgram_anim);
    delete shaderManager;
    delete modelManager;
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

    #ifdef __APPLE__
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        Window::width = fbWidth;
        Window::height = fbHeight;
    #else
        Window::width = width;
        Window::height = height;
    #endif

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

    // Set swap interval to 1 to stop flickering.
    glfwSwapInterval(1);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));
    // MiniMapCam = new Camera();
    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    //for minimap
    MiniMapCam = new Camera();
    MiniMapCam->SetOrtho(-10, 10, -10, 10, 0.1f, 100.0f); 
    MiniMapCam->SetLookAt(glm::vec3(0, 20, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

    // Call the resize callback to make sure things get drawn immediately.

    #ifdef __APPLE__
        // macOS: Use framebuffer size to handle Retina displays correctly
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
        // Cam->SetAspect(float(fbWidth) / float(fbHeight));
        Window::width = fbWidth;
        Window::height = fbHeight;
    #else
        Window::resizeCallback(window, width, height);
    #endif

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

void Window::activateRadarAbility() {
    showOthersOnMiniMap = true; 
    radarActive = true;        
    radarStartTime = std::chrono::steady_clock::now(); 
}

void Window::updateRadarAbility() {
    if (radarActive) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - radarStartTime);

        if (elapsed.count() > radarDuration) {
            showOthersOnMiniMap = false;  
            radarActive = false;
        }
    }
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    // Cam->Update();
    // if (Window::currentAbility == AbilityType::INVISIBILITY) {
    //     // Apply invisibility logic
    // }   

    if (character != nullptr) {
     
        Cam->Update(character->getPosition());

        character->update(animationPlayer);
    }
    // Cam->Update(cube->getPosition());

    // cube->update();
    NPC->update();

    // if (cube->didUseRadarAbility()) {
    //     activateRadarAbility();
    // }

    // updateRadarAbility();
    scene->update(animationPlayer);
}

// void Window::displayCallback(GLFWwindow* window) {
    
    // if (cube != nullptr) {
    //     // cube->handleContinuousInput(window);
    // // Cam->Update(cube->getPosition());
    // Cam->Update(character->getPosition());

    // // animationPlayer->fullUpdate();
    // // cube->update();
    // character->update(animationPlayer);
    // // if ((bro2->getState() != AnimState::FT_Walk) && (float) glfwGetTime() > 10){
    // //     bro2->setState(AnimState::FT_Walk);
    // }

//     scene->update(animationPlayer);
//     // model->update();
// }

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
    #ifdef __APPLE__
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    #else
        glViewport(0, 0, Window::width, Window::height);
    #endif

    // Render the object.
    // cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,false);
    // floor->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,true);
    // NPC->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    // artifact->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,false);
    floor->draw(Cam->GetViewProjectMtx(), shaderManager->getShader(RenderMode::BASE, false), true);

    int miniMapSize = 256;
    //glViewport(0, Window::height - miniMapSize, miniMapSize, miniMapSize); 
    // #ifdef __APPLE__
    //     glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    //     glViewport(0, fbHeight - miniMapSize, miniMapSize, miniMapSize); 
    // #else
    //     glViewport(0, Window::height - miniMapSize, miniMapSize, miniMapSize); 
    // #endif
    // glm::mat4 viewProj_miniMap = MiniMapCam->GetViewProjectMtx();
    // cube->draw(viewProj_miniMap, shaderProgram, false);
    // artifact->draw(viewProj_miniMap, shaderProgram, false);
    // floor->draw(viewProj_miniMap, shaderProgram, true);

    if (showOthersOnMiniMap){
        /*
        Drawing npc for test only, for multiplayer with networking, can do draw others here 
        something like for each cube in cubePtr, draw it ish
        */
        // NPC->draw(viewProj_miniMap, Window::shaderProgram);
    }
    // // animationPlayer->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    // // Render the object.
    // // cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,false);

    // floor->draw(Cam->GetViewProjectMtx(), shaderManager->getShader(RenderMode::BASE, false), true);

    scene->draw(Cam->GetViewProjectMtx(), shaderManager);
    // scene->draw(Cam->GetViewProjectMtx(), Window::shaderProgram_anim);
    // scene->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    // scene->draw(Cam->GetViewProjectMtx(), Window::shaderProgram_uv);
    // // model->draw(Cam->GetViewProjectMtx(), Window::shaderProgram_uv);
    
    character->draw(Cam->GetViewProjectMtx(), shaderManager);
    printf("character drawn\n");
    // Gets events, including input such as keyboard and mouse or window resizing.
    // glfwPollEvents();
    // Swap buffers.
    // glfwSwapBuffers(window);
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

            // if (key == GLFW_KEY_P && currentState == PLAYING) {
            //     currentState = IN_MINIGAME;
            // }


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
    //std::cout << Window::width << std::endl;

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

/*
void Window::applyServerState(char * data) {
    int numClients;
    memcpy(&numClients, data, sizeof(int));
    int offset = sizeof(numClients);
    printf("num clients is %d\n", numClients);
    for (int i = 0; i < numClients; i++) {
        //printf("there are %d clients", numClients);
        unsigned int currClient;
        memcpy(&currClient, &data[offset], sizeof(currClient));
        offset += sizeof(currClient);
        printf("curr client is %u\n", currClient);

        if (cubes.find(currClient) == cubes.end()) {
            addClient(currClient);
        }
        Cube* cubePtr = cubes[currClient];
        int cubeOffset = cubePtr->readFromArray(&data[offset]);
        offset += cubeOffset;
   
        if (currClient == client_id) {
            offset += Cam->readFromArray(&data[offset]);
            offset += MiniMapCam->readFromArray(&data[offset]);
            memcpy(&altDown, &data[offset], sizeof(bool));
            offset += sizeof(bool);
        }
        else {
            offset += (2 * (100) + 1);
        }
    }
}
*/

void Window::applyServerState(const StateUpdatePacket& packet) {
    int numClients = packet.numClients;
    
    for (const auto& clientPacketPtr : packet.clientPackets) {
        if (clientPacketPtr) {
            const InitPlayerPacket* initPacket = dynamic_cast<const InitPlayerPacket*>(clientPacketPtr.get());
            
            if (initPacket) {
                unsigned int currClient = initPacket->clientID;
                // printf("curr client is %u\n", client_id);
                
                if (characters.find(currClient) == characters.end()) {
                    addClient(currClient, initPacket->type);
                }
                Character* character = characters[currClient];
                character->updateFromPacket(*initPacket, animationPlayer);
                // character->model->update(animationPlayer);

                if (currClient == client_id) {
                    Cam->updateFromPacket(*initPacket, false);
                    MiniMapCam->updateFromPacket(*initPacket, true);
                    altDown = initPacket->altDown;
                    radarActive = initPacket->radarActive;
                    currentState = initPacket->currentState;
                }
            }
        }
    }

    for (const auto& npcPacketPtr : packet.npcPackets) {

        if (npcPacketPtr) {
            const NPCPacket* npcPacket = dynamic_cast<const NPCPacket*>(npcPacketPtr.get());

            if (npcPacket) {
                unsigned int npcID = npcPacket->npcID;
                // printf("curr npc is %u\n", npcID);
                
                if (npcs.find(npcID) == npcs.end()) {
                    addNPC(npcID);
                }
                NPCs* npc = npcs[npcID];
                npc->updateFromPacket(*npcPacket);
            }
        }
    }   
    glm::mat4 artifactModel;
    memcpy(glm::value_ptr(artifactModel), packet.artifactModel, sizeof(packet.artifactModel));
    artifact->setBaseModel(artifactModel);
}

void Window::render(GLFWwindow* window) {
    if (!initialized) return;

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
    std::map<unsigned int, Character*>::iterator playerIter;

    for (playerIter = characters.begin(); playerIter != characters.end(); playerIter++) {
        //printf("rendering cube for client %u\n", iter->first);
        // playerIter->second->model->update(animationPlayer);
        // playerIter->second->draw(Cam->GetViewProjectMtx(),shaderManager);
    }

    std::map<unsigned int, NPCs*>::iterator npcIter;

    for (npcIter = npcs.begin(); npcIter != npcs.end(); npcIter++) {
        //printf("rendering cube for client %u\n", iter->first);
        npcIter->second->draw(Cam->GetViewProjectMtx(), shaderManager->getShader(RenderMode::BASE, false));
    }

   //cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram,false);
    floor->draw(Cam->GetViewProjectMtx(), shaderManager->getShader(RenderMode::BASE, false), true);
    artifact->draw(Cam->GetViewProjectMtx(), shaderManager->getShader(RenderMode::BASE, false),false);

    int miniMapSize = 256;
    //glViewport(0, Window::height - miniMapSize, miniMapSize, miniMapSize); 
    #ifdef __APPLE__
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, fbHeight - miniMapSize, miniMapSize, miniMapSize); 
    #else
        glViewport(0, Window::height - miniMapSize, miniMapSize, miniMapSize); 
    #endif
    glm::mat4 viewProj_miniMap = MiniMapCam->GetViewProjectMtx();
    //cube->draw(viewProj_miniMap, Window::shaderProgram, false);
    // for (playerIter=cubes.begin(); playerIter != cubes.end(); playerIter++) {
    //     playerIter->second->draw(viewProj_miniMap, shaderManager->getShader(RenderMode::BASE, false), false);
    // }
    floor->draw(viewProj_miniMap, shaderManager->getShader(RenderMode::BASE, false), true);
    artifact->draw(viewProj_miniMap, shaderManager->getShader(RenderMode::BASE, false), false);

    if (radarActive){

        for (npcIter = npcs.begin(); npcIter != npcs.end(); npcIter++) {
            //printf("rendering cube for client %u\n", iter->first);
            npcIter->second->draw(viewProj_miniMap, shaderManager->getShader(RenderMode::BASE, false));
        }
        // NPC->draw(viewProj_miniMap, Window::shaderProgram);
    }

    // Gets events, including input such as keyboard and mouse or window resizing.
    // glfwPollEvents();
    // Swap buffers.
    // glfwSwapBuffers(window);
    
    if (altDown) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    /* cube->update();
    Cam->Update(cube->getPosition()); */
}

void Window::setClientID(const InitPlayerPacket& packet) {
    client_id = packet.clientID;
    
    printf("client id is %d\n", client_id);
}

void Window::addClient(unsigned int client, CharacterType type) {
    ModelType characterModel;

    if (type == CHARACTER_4) {
        characterModel = ModelType::SecurityGuard;
    } else {
        characterModel = ModelType::FemaleThief;
    }
    AnInstance* model = new AnInstance(modelManager->getModel(characterModel));
    Character* characterPtr = new Character(model);
    characters.insert(std::pair<unsigned int, Character*>(client, characterPtr));
    printf("init character for client %u\n", client);
}

void Window::addNPC(unsigned int npc) {
    NPCs* npcPtr = new NPCs(new Cube(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)));
    npcs.insert(std::pair<unsigned int, NPCs*>(npc, npcPtr));
    // printf("init cube for client %u\n", client);
}

void Window::removeClient(unsigned int client) {

    if (characters.find(client) != characters.end()) {
        Character* charPtr = characters[client];
        delete charPtr;
        characters.erase(client);
    }
}

void Window::setInitState(const InitPlayerPacket& packet) {
    client_id = packet.clientID;
    printf("client id init as %d\n", client_id);
    // addClient(packet.clientID);
    // cubes[packet.clientID]->updateFromPacket(packet);
    // Cam->updateFromPacket(packet, false);
    // MiniMapCam->updateFromPacket(packet, true);
    // altDown = packet.altDown;
    // initialized = true;
    currentState = packet.currentState;

    // if (currentState == WAITING) {
    //     initialized = true;
    // }
}

void Window::applyGuiUpdate(const GuiUpdatePacket& guiPacket) {
    currentState = guiPacket.currentState;

    if (currentState == WAITING) {
        initialized = true;
    }
}
