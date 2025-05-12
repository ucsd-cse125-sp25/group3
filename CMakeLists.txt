cmake_minimum_required(VERSION 3.10)
project(MiniPlatformer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files
file(GLOB SOURCES
    src/*.cpp
    main.cpp
)

# Include headers
include_directories(include)
include_directories(/opt/homebrew/opt/glfw/include)
include_directories(/opt/homebrew/opt/glew/include)
include_directories(/opt/homebrew/opt/glm/include)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Manually specify GLFW and GLEW libs
find_library(GLFW_LIBRARY NAMES glfw PATHS /opt/homebrew/opt/glfw/lib)
find_library(GLEW_LIBRARY NAMES GLEW PATHS /opt/homebrew/opt/glew/lib)

# macOS specific frameworks
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)

    set(PLATFORM_LIBS ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif()

# Executable
add_executable(game ${SOURCES})

# Link everything
target_link_libraries(game
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    ${PLATFORM_LIBS}
)
