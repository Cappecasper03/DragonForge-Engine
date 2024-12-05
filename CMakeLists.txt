cmake_minimum_required(VERSION 3.31.0)

project(Template)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(USE_FOLDERS true)

if(MSVC)
    add_compile_options(-MP -W0)

    add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

    add_compile_definitions($<$<CONFIG:Release>:NDEBUG>)
    add_compile_definitions($<$<CONFIG:Release>:RELEASE>)
    add_link_options($<$<CONFIG:Release>:/LTCG>)
else()
endif()

if(TRACY_ENABLE)
    add_compile_definitions(PROFILING)
    set(TRACY_ON_DEMAND true)
    set(TRACY_ONLY_LOCALHOST true)
    set(TRACY_VK_USE_SYMBOL_TABLE true)
endif()

add_subdirectory(libraries)

add_subdirectory(source)
