cmake_minimum_required(VERSION 3.16)
project(menv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(my_build_type "Debug")

file(GLOB SOURCES *.cpp src/*.cpp Networking/*.cpp packets/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*server\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*client\\.cpp$")

file(GLOB HEADERS *.h include/*.h Networking/*.h packets/*.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB MINIGAME_SOURCES
    "minigame/*.cpp"
    "minigame/src/*.cpp"
)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_subdirectory(dependencies/assimp ${CMAKE_BINARY_DIR}/assimp)

find_package(OpenGL REQUIRED)

include_directories(
    include
    minigame
    minigame/include
    ${ASSIMP_INCLUDE_DIRS}
    external/miniaudio
    external/soloud/include
)

link_directories(lib)

# ======================== FIXED SOLOUD SECTION =========================
file(GLOB SOLOUD_CORE_SOURCES external/soloud/src/core/*.cpp)
file(GLOB SOLOUD_BACKEND_SOURCES external/soloud/src/backend/miniaudio/*.cpp)
file(GLOB SOLOUD_WAV_SOURCES external/soloud/src/audiosource/wav/*.cpp)

set(MINIAUDIO_SOURCE ${CMAKE_SOURCE_DIR}/external/soloud/src/backend/miniaudio/soloud_miniaudio.cpp)
set(STB_VORBIS_SOURCE ${CMAKE_SOURCE_DIR}/external/stb_vorbis.c)

add_library(soloud STATIC
    ${SOLOUD_CORE_SOURCES}
    ${SOLOUD_BACKEND_SOURCES}
    ${SOLOUD_WAV_SOURCES}
    ${MINIAUDIO_SOURCE}
    ${STB_VORBIS_SOURCE}
)

target_compile_definitions(soloud PRIVATE 
    WITH_MINIAUDIO
    MA_NO_RUNTIME_LINKING
    MA_NO_NULL
    MA_NO_JACK
)


target_include_directories(soloud PUBLIC
    ${CMAKE_SOURCE_DIR}/external/soloud/include
    ${CMAKE_SOURCE_DIR}/external/miniaudio
    ${CMAKE_SOURCE_DIR}/external/soloud/src/core
)

if(WIN32)
    target_link_libraries(soloud PRIVATE winmm)
elseif(APPLE)
    find_library(AUDIO_TOOLBOX AudioToolbox)
    find_library(CORE_AUDIO CoreAudio)
    find_library(CORE_FOUNDATION CoreFoundation)
    target_link_libraries(soloud PRIVATE 
        ${AUDIO_TOOLBOX} 
        ${CORE_AUDIO}
        ${CORE_FOUNDATION}
    )
endif()
# =======================================================================

add_executable(server Networking/server.cpp ${SOURCES} ${MINIGAME_SOURCES} ${HEADERS} ${IMGUI_SOURCES})
add_executable(client Networking/client.cpp ${SOURCES} ${MINIGAME_SOURCES} ${HEADERS} ${IMGUI_SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES} ${MINIGAME_SOURCES} ${HEADERS} ${IMGUI_SOURCES} main.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE soloud)
target_link_libraries(server PRIVATE soloud)
target_link_libraries(client PRIVATE soloud)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends external)

if (APPLE)
    message(STATUS "Building on macOS (ARM)")
    set(CUSTOM_PATHS /opt/homebrew /usr/local)

    find_path(GLEW_INCLUDE_DIR GL/glew.h PATHS ${CUSTOM_PATHS} PATH_SUFFIXES include NO_DEFAULT_PATH)
    find_path(GLM_INCLUDE_DIR glm/glm.hpp PATHS ${CUSTOM_PATHS} PATH_SUFFIXES include NO_DEFAULT_PATH)
    find_library(GLEW_LIBRARY GLEW PATHS ${CUSTOM_PATHS} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    find_library(GLFW_LIBRARY glfw PATHS ${CUSTOM_PATHS} PATH_SUFFIXES lib NO_DEFAULT_PATH)

    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_VIDEO CoreVideo)

    if(GLEW_INCLUDE_DIR)
        target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIR})
        target_include_directories(server PRIVATE ${GLEW_INCLUDE_DIR})
        target_include_directories(client PRIVATE ${GLEW_INCLUDE_DIR})
    endif()

    if(GLEW_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARY})
        target_link_libraries(server PRIVATE ${GLEW_LIBRARY})
        target_link_libraries(client PRIVATE ${GLEW_LIBRARY})
    endif()

    if(GLFW_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARY})
        target_link_libraries(server PRIVATE ${GLFW_LIBRARY})
        target_link_libraries(client PRIVATE ${GLFW_LIBRARY})
    endif()

    target_link_libraries(${PROJECT_NAME} PRIVATE 
        assimp 
        ${OPENGL_LIBRARIES}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${CORE_VIDEO}
    )
    target_link_libraries(server PRIVATE 
        assimp 
        ${OPENGL_LIBRARIES}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${CORE_VIDEO}
    )
    target_link_libraries(client PRIVATE 
        assimp 
        ${OPENGL_LIBRARIES}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${CORE_VIDEO}
    )

elseif (WIN32)
    message(STATUS "Building on Windows")
    link_directories(lib)

    set(WINDOWS_LIBS PRIVATE ${OPENGL_LIBRARIES} glew32s.lib glfw3 assimp)

    target_link_libraries(${PROJECT_NAME} ${WINDOWS_LIBS})
    target_link_libraries(server ${WINDOWS_LIBS})
    target_link_libraries(client ${WINDOWS_LIBS})
endif()

add_custom_target(CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders")
add_dependencies(menv CopyShaders)
add_dependencies(server CopyShaders)
add_dependencies(client CopyShaders)

add_custom_target(CopyModels ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/models" "${CMAKE_BINARY_DIR}/models")

add_custom_target(CopyTextures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/textures" "${CMAKE_BINARY_DIR}/textures")

if (WIN32)
    add_custom_command(TARGET menv POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/assimp/bin/${my_build_type}/assimp-vc143-mtd.dll ${CMAKE_BINARY_DIR}/${my_build_type})
endif()

add_dependencies(menv CopyShaders CopyModels CopyTextures)
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
