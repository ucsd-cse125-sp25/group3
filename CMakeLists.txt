cmake_minimum_required(VERSION 3.16)

if(APPLE)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

project(menv)

set(my_build_type "Debug")

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

add_subdirectory(dependencies/assimp ${CMAKE_BINARY_DIR}/assimp)

# Require GL
find_package(OpenGL REQUIRED)

include_directories(
    include
    ${ASSIMP_INCLUDE_DIRS}
)

link_directories(
    lib
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

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

    target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

elseif (WIN32)
    message(STATUS "Building on Windows")

    # Link libraries
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3 assimp)
endif()

add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/shaders"
)

add_custom_target(CopyModels ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/models"
	"${CMAKE_BINARY_DIR}/models"
)

add_custom_target(CopyTextures ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/textures"
	"${CMAKE_BINARY_DIR}/textures"
)

if(DEFINED CMAKE_BUILD_TYPE)
    set(my_build_type CMAKE_BUILD_TYPE)
endif()

if (WIN32)
    add_custom_command(TARGET menv
        COMMENT "Copy Assimp DLL"
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/_deps/assimp-build/bin/${my_build_type}/assimp-vc143-mtd.dll ${CMAKE_BINARY_DIR}/${my_build_type}    
    )
endif()

add_dependencies(menv CopyShaders CopyModels CopyTextures)


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

# Move assets to .exe