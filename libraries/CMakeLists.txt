project(libraries)

set(CMAKE_FOLDER libraries/assimp)
set(BUILD_SHARED_LIBS false)
set(ASSIMP_BUILD_TESTS false)
add_subdirectory(assimp)

set(CMAKE_FOLDER libraries/fmt)
set(FMT_TEST false)
add_subdirectory(fmt)

set(CMAKE_FOLDER libraries/freetype)
add_subdirectory(freetype)

set(CMAKE_FOLDER libraries/glfw)
set(GLFW_BUILD_EXAMPLES false)
set(GLFW_BUILD_TESTS false)
add_subdirectory(glfw)

set(CMAKE_FOLDER libraries/glm)
add_subdirectory(glm)

set(TRACY_ENABLE false)
set(CMAKE_FOLDER libraries/tracy)
add_subdirectory(tracy)

set(CMAKE_FOLDER libraries/VulkanMemoryAllocator-Hpp)
add_subdirectory(VulkanMemoryAllocator-Hpp)
