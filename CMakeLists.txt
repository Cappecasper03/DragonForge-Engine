cmake_minimum_required(VERSION 3.31.0)

project(DragonForge-Engine)

set(CMAKE_CXX_STANDARD 23)
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

add_compile_definitions(VULKAN_HPP_NO_EXCEPTIONS)
add_compile_definitions(VULKAN_HPP_DISPATCH_LOADER_DYNAMIC)
add_compile_definitions(VULKAN_NO_PROTOTYPES)
add_compile_definitions(VULKAN_HPP_NO_NODISCARD_WARNINGS)

if(TRACY_ENABLE)
    add_compile_definitions(PROFILING)
endif()
add_compile_definitions(TRACY_ONLY_LOCALHOST)
add_compile_definitions(TRACY_VK_USE_SYMBOL_TABLE)

add_subdirectory(libraries)

add_subdirectory(source)
