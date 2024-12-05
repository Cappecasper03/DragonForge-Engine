project(source)

set(CMAKE_FOLDER source)

if(MSVC)
    add_compile_options(-W4 -WX)
else()
endif()

add_subdirectory(application)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT application)
