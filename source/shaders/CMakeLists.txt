cmake_minimum_required(VERSION 3.30.3)

project(shaders)

file(GLOB_RECURSE OPENGL_FILES
    opengl/**.vert
    opengl/**.frag
)

file(GLOB_RECURSE VULKAN_FILES
    vulkan/**.vert
    vulkan/**.frag
)

add_custom_target(${PROJECT_NAME} SOURCES ${OPENGL_FILES} ${VULKAN_FILES})

source_group(
    TREE ${PROJECT_SOURCE_DIR}
    PREFIX ""
    FILES ${OPENGL_FILES} ${VULKAN_FILES}
)

foreach(SHADER ${OPENGL_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SHADER_OUTPUT_NAME ${SHADER_NAME})
    add_custom_command(
        MAIN_DEPENDENCY ${SHADER}
        OUTPUT ${CMAKE_SOURCE_DIR}/game/binaries/shaders/opengl/${SHADER_OUTPUT_NAME}
        COMMAND echo F|xcopy /f \"${SHADER}\" \"${CMAKE_SOURCE_DIR}/game/binaries/shaders/opengl/${SHADER_OUTPUT_NAME}\"
    )
endforeach()

foreach(SHADER ${VULKAN_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SHADER_OUTPUT_NAME ${SHADER_NAME}.spv)
    add_custom_command(
        MAIN_DEPENDENCY ${SHADER}
        OUTPUT ${CMAKE_SOURCE_DIR}/game/binaries/shaders/vulkan/${SHADER_OUTPUT_NAME}
        COMMAND ${CMAKE_SOURCE_DIR}/utilities/vulkan/glslc.exe ${SHADER} -o ${CMAKE_SOURCE_DIR}/game/binaries/shaders/vulkan/${SHADER_OUTPUT_NAME}
    )
endforeach()
