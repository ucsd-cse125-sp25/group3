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

# add_subdirectory(lib/assimp)
# add_subdirectory(lib/assimp ${CMAKE_BINARY_DIR}/assimp)

add_subdirectory(dependencies/assimp ${CMAKE_BINARY_DIR}/assimp)

# Require GL
find_package(OpenGL REQUIRED)

# Add include directories
# include_directories(
#     include
#     "lib/assimp/include"
# )

include_directories(
    include
    ${ASSIMP_INCLUDE_DIRS}
)

# Add library directories
# link_directories(
#     lib
#     lib/assimp
# )

link_directories(
    lib
    # ${ASSIMP_INCLUDE_DIRS}
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

add_custom_target(CopyModels ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/models"
	"${CMAKE_BINARY_DIR}/models"
)

add_custom_target(Copy)

add_dependencies(menv CopyShaders CopyModels)

# cmake_minimum_required(VERSION 3.16)

# if(APPLE)
#     set(CMAKE_CXX_STANDARD 11)
#     set(CMAKE_CXX_STANDARD_REQUIRED ON)
# endif()

# project(menv)

# add_subdirectory(dependencies/assimp ${CMAKE_BINARY_DIR}/assimp)

# # Add source files
# file(
#     GLOB SOURCES
#     *.cpp
#     src/*.cpp
# )

# # Add header files
# file(
#     GLOB HEADERS
#     *.h
#     include/*.h
# )

# # Require GL
# find_package(OpenGL REQUIRED)

# # Add executable
# add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# # target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)
# # Set include and lib dirs (platform-specific)
# include_directories(
#     include
#     ${ASSIMP_INCLUDE_DIRS}
# )

# if (APPLE)
#     message(STATUS "Building on macOS (ARM)")

#     # Base paths differ
#     set(CUSTOM_PATHS
#         /opt/homebrew
#         /usr/local
#     )

#     # Find include path (e.g., for GLEW headers)
#     find_path(GLEW_INCLUDE_DIR GL/glew.h
#         PATHS ${CUSTOM_PATHS}
#         PATH_SUFFIXES include
#         NO_DEFAULT_PATH
#     )

#     # Find libraries
#     find_library(GLEW_LIBRARY GLEW
#         PATHS ${CUSTOM_PATHS}
#         PATH_SUFFIXES lib
#         NO_DEFAULT_PATH
#     )

#     find_library(GLFW_LIBRARY glfw
#         PATHS ${CUSTOM_PATHS}
#         PATH_SUFFIXES lib
#         NO_DEFAULT_PATH
#     )

#     # find_library(ASSIMP


#     # )

#     # Use system OpenGL
#     find_package(OpenGL REQUIRED)

#     # Apply if found
#     if(GLEW_INCLUDE_DIR)
#         target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIR})
#     endif()

#     if(GLEW_LIBRARY)
#         target_link_libraries(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARY})
#     endif()

#     if(GLFW_LIBRARY)
#         target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARY})
#     endif()

#     target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

# target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

# elseif (WIN32)
#     message(STATUS "Building on Windows")
#     link_directories(
#         lib
#     )
#     target_link_libraries(${PROJECT_NAME}
#         ${OPENGL_LIBRARIES}
#         glew32s.lib
#         glfw3
#         assimp
#     )
# endif()

# # Move assets to .exe
# add_custom_target(CopyShaders ALL
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 	"${PROJECT_SOURCE_DIR}/shaders"
# 	"${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders"
# )

# add_custom_target(CopyModels ALL
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 	"${PROJECT_SOURCE_DIR}/models"
# 	"${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/models"
# )

# add_dependencies(menv CopyShaders)


# # find_package(Doxygen)
# # if(DOXYGEN_FOUND)
# #     set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
# #     set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

# #     add_custom_target(doc_doxygen
# #         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
# #         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
# #         COMMENT "Generating API documentation with Doxygen"
# #         VERBATIM)
# # endif()
