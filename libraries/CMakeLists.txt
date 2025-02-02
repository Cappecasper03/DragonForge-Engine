project(libraries)

# Packages
find_package(Vulkan REQUIRED)

# Submodules
set(CMAKE_FOLDER libraries/assimp)
set(BUILD_SHARED_LIBS false)
set(ASSIMP_BUILD_TESTS false)
add_subdirectory(assimp)

set(CMAKE_FOLDER libraries/fmt)
set(FMT_TEST false)
add_subdirectory(fmt)

set(CMAKE_FOLDER libraries/freetype)
add_subdirectory(freetype)

set(CMAKE_FOLDER libraries/glm)
add_subdirectory(glm)

set(CMAKE_FOLDER libraries/sdl)
add_subdirectory(sdl)

set(CMAKE_FOLDER libraries/tracy)
add_subdirectory(tracy)

set(CMAKE_FOLDER libraries/VulkanMemoryAllocator-Hpp)
add_subdirectory(VulkanMemoryAllocator-Hpp/Vulkan-Headers)
add_subdirectory(VulkanMemoryAllocator-Hpp/VulkanMemoryAllocator SYSTEM)
add_subdirectory(VulkanMemoryAllocator-Hpp)

# Non-submodules
set(CMAKE_FOLDER libraries/glad)
add_subdirectory(glad)

set(CMAKE_FOLDER libraries/imgui)
add_subdirectory(imgui)

set(CMAKE_FOLDER libraries/stb)
add_subdirectory(stb)
