cmake_minimum_required(VERSION 3.16)

if(APPLE)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

project(menv)

# Add source files
file(
    GLOB SOURCES
    *.cpp
    src/*.cpp
)

# Add header files
file(
    GLOB HEADERS
    *.h
    include/*.h
)

add_subdirectory(lib/assimp)

# Require GL
find_package(OpenGL REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)
# Set include and lib dirs (platform-specific)
include_directories(
    include
    "lib/assimp/include"
)

if (APPLE)
    message(STATUS "Building on macOS (ARM)")

    # Base paths differ
    set(CUSTOM_PATHS
        /opt/homebrew
        /usr/local
    )

    # Find include path (e.g., for GLEW headers)
    find_path(GLEW_INCLUDE_DIR GL/glew.h
        PATHS ${CUSTOM_PATHS}
        PATH_SUFFIXES include
        NO_DEFAULT_PATH
    )

    # Find libraries
    find_library(GLEW_LIBRARY GLEW
        PATHS ${CUSTOM_PATHS}
        PATH_SUFFIXES lib
        NO_DEFAULT_PATH
    )

    find_library(GLFW_LIBRARY glfw
        PATHS ${CUSTOM_PATHS}
        PATH_SUFFIXES lib
        NO_DEFAULT_PATH
    )

    # find_library(ASSIMP


    # )

    # Use system OpenGL
    find_package(OpenGL REQUIRED)

    # Apply if found
    if(GLEW_INCLUDE_DIR)
        target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIR})
    endif()

    if(GLEW_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARY})
    endif()

    if(GLFW_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARY})
    endif()

    # if(ASSIMP)
    #     target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMP})
    # endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

elseif (WIN32)
    message(STATUS "Building on Windows")
    link_directories(
        lib
        lib/assimp
    )
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        glew32s.lib
        glfw3
        assimp
    )
endif()

# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/shaders"
)
add_dependencies(menv CopyShaders)


find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()
