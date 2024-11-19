project(engine CXX)

file(GLOB_RECURSE SOURCE_FILES "*.h" "*.cpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ../../source)
