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

include_directories(
    ../../source
)

file(GLOB_RECURSE INCLUDE_DIRS LIST_DIRECTORIES true
    ../../libraries/*
)

include_directories(
    SYSTEM
    ${INCLUDE_DIRS}/include
    ${INCLUDE_DIRS}/source
)
