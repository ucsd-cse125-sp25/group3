# Install script for directory: /Users/jessieouyang/group3/dependencies/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.4.3" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/Users/jessieouyang/group3/assimp/bin/libassimp.5.4.3.dylib"
    "/Users/jessieouyang/group3/assimp/bin/libassimp.5.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.5.4.3.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.5.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -x "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.4.3" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/jessieouyang/group3/assimp/bin/libassimp.dylib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/anim.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/aabb.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/ai_assert.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/camera.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/color4.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/color4.inl"
    "/Users/jessieouyang/group3/assimp/code/../include/assimp/config.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/ColladaMetaData.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/commonMetaData.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/defs.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/cfileio.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/light.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/material.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/material.inl"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/matrix3x3.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/matrix3x3.inl"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/matrix4x4.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/matrix4x4.inl"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/mesh.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/ObjMaterial.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/pbrmaterial.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/GltfMaterial.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/postprocess.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/quaternion.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/quaternion.inl"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/scene.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/metadata.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/texture.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/types.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/vector2.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/vector2.inl"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/vector3.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/vector3.inl"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/version.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/cimport.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/AssertHandler.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/importerdesc.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Importer.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/DefaultLogger.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/ProgressHandler.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/IOStream.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/IOSystem.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Logger.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/LogStream.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/NullLogger.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/cexport.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Exporter.hpp"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/DefaultIOStream.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/DefaultIOSystem.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/SceneCombiner.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/fast_atof.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/qnan.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/BaseImporter.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Hash.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/ParsingUtils.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/StreamReader.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/StreamWriter.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/StringComparison.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/StringUtils.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/SGSpatialSort.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/GenericProperty.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/SpatialSort.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/SmallVector.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/SmoothingGroups.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/SmoothingGroups.inl"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/StandardShapes.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/RemoveComments.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Subdivision.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Vertex.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/LineSplitter.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/TinyFormatter.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Profiler.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/LogAux.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Bitmap.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/XMLTools.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/IOStreamBuffer.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/CreateAnimMesh.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/XmlParser.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/BlobIOSystem.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/MathFunctions.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Exceptional.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/ByteSwapper.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Compiler/poppack1.h"
    "/Users/jessieouyang/group3/dependencies/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

