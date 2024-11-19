cmake_minimum_required(VERSION 3.31.0)

project(DragonForge-Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(USE_FOLDERS true)

if(MSVC)
    add_compile_options(/MP /W4 /WX)

    add_compile_options($<$<CONFIG:Debug>:-DDEBUG>)
    add_compile_options($<$<CONFIG:Debug>:/Zi>)

    add_compile_options($<$<CONFIG:Release>:-DNDEBUG>)
    add_compile_options($<$<CONFIG:Release>:-DRELEASE>)
    add_compile_options($<$<CONFIG:Release>:/0x>)
    add_link_options($<$<CONFIG:Release>:/LTCG>)
else()
endif()

add_subdirectory(source)

add_subdirectory(libraries)
