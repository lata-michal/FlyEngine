add_library(FlyEngine STATIC
    src/source/BloomEffect.cpp
    src/source/Camera.cpp
    src/source/FrameBuffer.cpp
    src/source/Mesh.cpp
    src/source/Model.cpp
    src/source/Shader.cpp
    src/source/Texture.cpp
    src/source/Window.cpp
    src/source/pch.cpp
    src/vendor/glm/detail/glm.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/GLAD/src/glad.c
)

target_include_directories(FlyEngine PUBLIC
    src/include
    src/vendor
    ${CMAKE_SOURCE_DIR}/Dependencies/GLFW/include
    ${CMAKE_SOURCE_DIR}/Dependencies/GLAD/include
    ${CMAKE_SOURCE_DIR}/Dependencies/ASSIMP/assimp-5.2.5/include
    ${CMAKE_SOURCE_DIR}/Dependencies/ASSIMP/assimp-5.2.5/contrib
)

target_compile_definitions(FlyEngine PUBLIC
    GLFW_INCLUDE_NONE
    _CRT_SECURE_NO_WARNINGS
    WIN32_LEAN_AND_MEAN
)

if(MSVC)
    target_compile_options(FlyEngine PRIVATE /wd6255 /wd4312)
endif()

target_link_libraries(FlyEngine PUBLIC
    glfw
    assimp
    OpenGL::GL
)

# Precompiled Headers - Only for C++ files
target_precompile_headers(FlyEngine PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/include/pch.h>
)

# Language standard (redundant if set in root but good to be explicit)
set_target_properties(FlyEngine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
