cmake_minimum_required(VERSION 3.30.3)

file(GLOB LIBRARY_DIRS LIST_DIRECTORIES true *)

foreach(LIBRARY_PATH ${LIBRARY_DIRS})
    if(IS_DIRECTORY ${LIBRARY_PATH})
        get_filename_component(LIBRARY_NAME ${LIBRARY_PATH} NAME)

        project(${LIBRARY_NAME})

        file(GLOB_RECURSE LIBRARY_FILES
            ${LIBRARY_PATH}/include/**
            ${LIBRARY_PATH}/source/**
        )

        add_library(${PROJECT_NAME} ${LIBRARY_FILES})

        source_group(
            TREE ${LIBRARY_PATH}
            PREFIX ""
            FILES ${LIBRARY_FILES}
        )

        include_directories(
            ${LIBRARY_PATH}/include
            ${LIBRARY_PATH}/source
        )

        add_compile_options(
            /W0
        )
    endif()
endforeach()
