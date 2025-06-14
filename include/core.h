#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum GameState {
    INIT,
    START_MENU,
    CHARACTER_SELECTION,
    WAITING,
    PLAYING,
    IN_MINIGAME,
    WIN_CONDITION
};

extern GameState currentState;
