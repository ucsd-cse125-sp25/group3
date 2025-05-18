cmake_minimum_required(VERSION 3.16)

if(APPLE)
    #set(CMAKE_CXX_STANDARD 11)
    #set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

project(menv)

# Add source files
set(
    SOURCES
    packets/Packet.cpp
    packets/CursorPacket.cpp
    packets/InitPacket.cpp
    packets/KeyPacket.cpp
    packets/StateUpdatePacket.cpp
    packets/PacketFactory.cpp
    src/Camera.cpp
    src/Cube.cpp
    src/Shader.cpp
    src/Tokenizer.cpp
    src/Window.cpp
    src/ClientLogic.cpp
    src/ServerLogic.cpp
    Networking/ServerGame.cpp 
    Networking/ServerNetwork.cpp 
    Networking/NetworkServices.cpp
    Networking/ClientGame.cpp 
    Networking/ClientNetwork.cpp
)

# Add header files
set(
    HEADERS
    packets/Packet.h
    packets/CursorPacket.h
    packets/InitPacket.h
    packets/KeyPacket.h
    packets/StateUpdatePacket.h
    packets/PacketFactory.h
    include/core.h
    include/Camera.h
    include/Cube.h
    include/Shader.h
    include/Tokenizer.h
    include/Window.h
    include/ClientLogic.h
    include/ServerLogic.h
    Networking/ServerGame.h 
    Networking/ServerNetwork.h
    Networking/NetworkServices.h
    Networking/ClientGame.h 
    Networking/ClientNetwork.h
)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS})
add_executable(server Networking/server.cpp ${SOURCES} ${HEADERS})
add_executable(client Networking/client.cpp ${SOURCES} ${HEADERS})
# Link libraries

# target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)
# Set include and lib dirs (platform-specific)
include_directories(include)

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

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(server PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(client PRIVATE ${OPENGL_LIBRARIES})

elseif (WIN32)
    message(STATUS "Building on Windows")
    link_directories(lib)
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        glew32s.lib
        glfw3
    )
    target_link_libraries(server
        ${OPENGL_LIBRARIES}
        glew32s.lib
        glfw3
    )
    target_link_libraries(client
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
add_dependencies(server CopyShaders)
add_dependencies(client CopyShaders)


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

# build GoogleTest code
enable_testing()

add_executable(
  Packet_test
  ./packets/Packet_test.cpp
  ./packets/Packet.cpp
)
target_link_libraries(
  Packet_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Packet_test)