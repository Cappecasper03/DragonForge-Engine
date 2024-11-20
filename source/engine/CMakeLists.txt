project(engine CXX)

file(GLOB_RECURSE SOURCE_FILES "*.h" "*.cpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ../../source)

target_link_libraries(${PROJECT_NAME} INTERFACE assimp)
target_link_libraries(${PROJECT_NAME} INTERFACE fmt)
target_link_libraries(${PROJECT_NAME} INTERFACE freetype)
target_link_libraries(${PROJECT_NAME} INTERFACE glfw)
target_link_libraries(${PROJECT_NAME} INTERFACE glm)
target_link_libraries(${PROJECT_NAME} INTERFACE TracyClient)
target_link_libraries(${PROJECT_NAME} INTERFACE VulkanMemoryAllocator-Hpp)

