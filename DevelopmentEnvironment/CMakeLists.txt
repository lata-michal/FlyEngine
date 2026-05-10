add_library(DevelopmentEnvironment STATIC
    src/source/Scene.cpp
)

target_include_directories(DevelopmentEnvironment PUBLIC
    src/include
)

target_link_libraries(DevelopmentEnvironment PUBLIC
    FlyEngine
)

set_target_properties(DevelopmentEnvironment PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
