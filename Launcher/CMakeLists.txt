add_executable(Launcher WIN32
    src/source/Application.cpp
)

target_link_libraries(Launcher PRIVATE
    DevelopmentEnvironment
    comctl32
)

# Resources
add_custom_command(TARGET Launcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:Launcher>/res
    COMMENT "Copying resources to build directory..."
)

set_target_properties(Launcher PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
