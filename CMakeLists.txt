cmake_minimum_required(VERSION 3.30.3)

project(DragonForge-Engine)

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES WindowsDebug)
    list(APPEND CMAKE_CONFIGURATION_TYPES WindowsProfiling)
    list(APPEND CMAKE_CONFIGURATION_TYPES WindowsRelease)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_definitions(
    $<$<CONFIG:WindowsDebug>:DEBUG>

    $<$<CONFIG:WindowsProfiling>:PROFILING>
    $<$<CONFIG:WindowsProfiling>:RELEASE>
    $<$<CONFIG:WindowsProfiling>:NDEBUG>

    $<$<CONFIG:WindowsRelease>:RELEASE>
    $<$<CONFIG:WindowsRelease>:NDEBUG>
)

add_compile_options(
    /MP
    /WX

    $<$<CONFIG:WindowsDebug>:/Wall>
)

add_subdirectory(source/shaders)
add_subdirectory(source/engine)
add_subdirectory(source/application)
