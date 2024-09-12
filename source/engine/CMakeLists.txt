cmake_minimum_required(VERSION 3.30.3)

project(engine)

file(GLOB_RECURSE SOURCE_FILES
    *.h
    *.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

source_group(
    TREE ${PROJECT_SOURCE_DIR}
    PREFIX ""
    FILES ${SOURCE_FILES}
)

include_directories(../source)
