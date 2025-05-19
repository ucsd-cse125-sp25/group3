#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

enum GameState {
    START_MENU,
    CHARACTER_SELECTION,
    PLAYING,
    IN_MINIGAME
};

extern GameState currentState;