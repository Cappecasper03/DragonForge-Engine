project(shaders)

file(GLOB_RECURSE OPENGL_FILES "opengl/*.vert" "opengl/*.frag")
file(GLOB_RECURSE VULKAN_FILES "vulkan/*.vert" "vulkan/*.frag")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${OPENGL_FILES} ${VULKAN_FILES})

add_custom_target(${PROJECT_NAME} SOURCES ${OPENGL_FILES} ${VULKAN_FILES})
