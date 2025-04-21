cmake_minimum_required(VERSION 3.16)

if(APPLE)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

project(menv)

# Add source files
set(
    SOURCES
    main.cpp
    src/Camera.cpp
    src/Cube.cpp
    src/Shader.cpp
    src/Tokenizer.cpp
    src/Window.cpp
)

# Add header files
set(
    HEADERS
    include/core.h
    include/Camera.h
    include/Cube.h
    include/Shader.h
    include/Tokenizer.h
    include/Window.h
)

# Require GL
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
    include
)

# Add library directories
link_directories(
    lib
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries

# target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)
# Set include and lib dirs (platform-specific)
include_directories(include)

if (APPLE)
    message(STATUS "Building on macOS (ARM)")

    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)

    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        /opt/homebrew/lib/libGLEW.dylib
        /opt/homebrew/lib/libglfw.dylib
    )
elseif (WIN32)
    message(STATUS "Building on Windows")
    link_directories(lib)
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        glew32s.lib
        glfw3
    )
endif()

# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/shaders"
)
add_dependencies(menv CopyShaders)