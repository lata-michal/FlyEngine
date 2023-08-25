# Install script for directory: C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.2.4-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/build/lib/Debug/assimp-vc143-mtd.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/build/lib/Release/assimp-vc143-mt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/build/lib/MinSizeRel/assimp-vc143-mt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/build/lib/RelWithDebInfo/assimp-vc143-mt.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/anim.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/aabb.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/ai_assert.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/camera.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/color4.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/color4.inl"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/build/code/../include/assimp/config.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/ColladaMetaData.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/commonMetaData.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/defs.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/cfileio.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/light.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/material.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/material.inl"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/matrix3x3.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/matrix3x3.inl"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/matrix4x4.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/matrix4x4.inl"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/mesh.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/ObjMaterial.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/pbrmaterial.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/GltfMaterial.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/postprocess.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/quaternion.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/quaternion.inl"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/scene.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/metadata.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/texture.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/types.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/vector2.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/vector2.inl"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/vector3.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/vector3.inl"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/version.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/cimport.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/importerdesc.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Importer.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/DefaultLogger.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/ProgressHandler.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/IOStream.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/IOSystem.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Logger.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/LogStream.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/NullLogger.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/cexport.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Exporter.hpp"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/DefaultIOStream.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/DefaultIOSystem.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/ZipArchiveIOSystem.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/SceneCombiner.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/fast_atof.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/qnan.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/BaseImporter.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Hash.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/MemoryIOWrapper.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/ParsingUtils.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/StreamReader.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/StreamWriter.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/StringComparison.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/StringUtils.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/SGSpatialSort.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/GenericProperty.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/SpatialSort.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/SkeletonMeshBuilder.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/SmallVector.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/SmoothingGroups.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/SmoothingGroups.inl"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/StandardShapes.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/RemoveComments.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Subdivision.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Vertex.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/LineSplitter.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/TinyFormatter.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Profiler.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/LogAux.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Bitmap.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/XMLTools.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/IOStreamBuffer.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/CreateAnimMesh.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/XmlParser.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/BlobIOSystem.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/MathFunctions.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Exceptional.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/ByteSwapper.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Compiler/pushpack1.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Compiler/poppack1.h"
    "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/build/code/Debug/assimp-vc143-mtd.pdb")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "C:/Dev/Visual Studio/OpenGL-CppCourse/Dependencies/ASSIMP/assimp-5.2.5/build/code/RelWithDebInfo/assimp-vc143-mt.pdb")
  endif()
endif()

