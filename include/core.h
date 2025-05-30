#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"

enum GameState {
    INIT,
    START_MENU,
    CHARACTER_SELECTION,
    WAITING,
    PLAYING,
    IN_MINIGAME
};

extern GameState currentState;
