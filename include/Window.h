#pragma once

#include "Camera.h"
#include "Cube.h"
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "core.h"
#include "NPCs.h"
#include <map>
#include "../packets/StateUpdatePacket.h"
#include "../packets/InitPlayerPacket.h"
#include "../packets/GuiUpdatePacket.h"

enum class AbilityType {
    NONE,
    INVISIBILITY,
    SPEED_UP
};
#include "Character.h"

#include "Skeleton.h"
#include "Animation.h"
#include "AnimationPlayer.h"

#include "ShaderManager.h"
#include "TextureManager.h"
#include "ModelManager.h"

class Window {
public:
    static unsigned int client_id;
    static bool initialized;
    static GameState currentState;
    static bool stateChanged;

    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;
    //static Camera* MiniMapCam;

    static AbilityType currentAbility;

    // Objects to render
    static Cube* cube;
    static std::map<unsigned int, Cube*> cubes;
    static Cube* floor;
    static NPCs* NPC;
    static std::map<unsigned int, NPCs*> npcs;
    static Cube* artifact;

    static Character* character;
    static std::map<unsigned int, Character*> characters;

    static Scene* scene;
    // static Model* model;

    static AnimationPlayer* animationPlayer;

    // Shader Program
    // static GLuint shaderProgram;
    // static GLuint shaderProgram_uv;
    // static GLuint shaderProgram_anim;
    static ShaderManager* shaderManager;
    static ModelManager* modelManager;
    static TextureManager* textureManager;


    //track the mouse
    static bool altDown;
    static bool firstMouse;

    // Act as Constructors and desctructors
    static bool initializeProgram();
    static bool initializeObjects();
    static void cleanUp();

    // for the Window
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    // update and draw functions
    static void idleCallback();
    static void displayCallback(GLFWwindow*);

    // helper to reset the camera
    static void resetCamera();

    // callbacks - for interaction
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);

    static void render(GLFWwindow* window);
    static void setClientID(const InitPlayerPacket& packet);
    static void addClient(unsigned int client);
    static void addNPC(unsigned int npc);
    static void removeClient(unsigned int client);
    static void setInitState(const InitPlayerPacket& packet);
    static void applyServerState(const StateUpdatePacket& packet);
    static void applyGuiUpdate(const GuiUpdatePacket& guiPacket);

    // for show-on-map ability 
    static void updateRadarAbility();
    static void activateRadarAbility() ;
    static bool showOthersOnMiniMap;
    static bool radarActive;
    static std::chrono::steady_clock::time_point radarStartTime;
    static const int radarDuration = 5;
};