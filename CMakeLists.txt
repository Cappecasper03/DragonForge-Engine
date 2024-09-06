cmake_minimum_required(VERSION 3.30.3)

project(DragonForge-Engine)

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES WindowsDebug)
    list(APPEND CMAKE_CONFIGURATION_TYPES WindowsProfiling)
    list(APPEND CMAKE_CONFIGURATION_TYPES WindowsRelease)
endif()

add_subdirectory(source/application)
