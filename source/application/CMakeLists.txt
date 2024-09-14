cmake_minimum_required(VERSION 3.30.3)

project(application)

file(GLOB_RECURSE SRC_FILES
    *.h
    *.cpp
    settings/**.rc
)

add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

source_group(
    TREE ${PROJECT_SOURCE_DIR}
    PREFIX ""
    FILES ${SRC_FILES}
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    application
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    engine
)

set(CMAKE_EXE_LINKER_FLAGS_WINDOWSDEBUG "${CMAKE_EXE_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_WINDOWSPROFILING "${CMAKE_EXE_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_WINDOWSRELEASE "${CMAKE_EXE_FLAGS_DEBUG}")

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY OUTPUT_NAME ${CMAKE_PROJECT_NAME}
)
set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:WindowsDebug>:Debug>
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/game/binaries)
