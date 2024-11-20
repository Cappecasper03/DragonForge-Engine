project(stb)

file(GLOB SOURCE_FILES "stb/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/stb FILES ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME} INTERFACE stb)
