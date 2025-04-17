cmake_minimum_required(VERSION 3.16)

project(menv)

# Add source files
file(
    GLOB SOURCES
    *.cpp
    src/*.cpp
    # main.cpp
    # src/Camera.cpp
    # src/Cube.cpp
    # src/Shader.cpp
    # src/Tokenizer.cpp
    # src/Window.cpp
)

# Add header files
file(
    GLOB HEADERS
    *.h
    include/*.h
    # include/core.h
    # include/Camera.h
    # include/Cube.h
    # include/Shader.h
    # include/Tokenizer.h
    # include/Window.h
)

add_subdirectory(lib/assimp)

# Require GL
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
    include
    "lib/assimp/include"
)

# Add library directories
link_directories(
    lib
    lib/assimp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3 assimp)

# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/shaders"
)
add_dependencies(menv CopyShaders)