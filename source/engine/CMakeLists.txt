cmake_minimum_required(VERSION 3.30.3)

project(engine)

file(GLOB_RECURSE SRC_FILES
    *.h
    *.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

source_group(
    TREE ${PROJECT_SOURCE_DIR}
    PREFIX ""
    FILES ${SRC_FILES}
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ../../source
)

file(GLOB LIBRARY_NAMES
    LIST_DIRECTORIES true
    RELATIVE ${CMAKE_SOURCE_DIR}/libraries
    ../../libraries/*
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    ${LIBRARY_NAMES}
)
