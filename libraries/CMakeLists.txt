cmake_minimum_required(VERSION 3.30.3)

file(GLOB LIBRARY_DIRS LIST_DIRECTORIES true *)

foreach(LIBRARY_PATH ${LIBRARY_DIRS})
    if(IS_DIRECTORY ${LIBRARY_PATH})
        get_filename_component(LIBRARY_NAME ${LIBRARY_PATH} NAME)

        project(${LIBRARY_NAME})

        file(GLOB_RECURSE INCLUDE_FILES
            ${LIBRARY_PATH}/include/**.hpp
            ${LIBRARY_PATH}/include/**.h
            ${LIBRARY_PATH}/include/**.cpp
        )

        file(GLOB_RECURSE SRC_FILES
            ${LIBRARY_PATH}/source/**.hpp
            ${LIBRARY_PATH}/source/**.h
            ${LIBRARY_PATH}/source/**.cpp
        )

        add_library(${PROJECT_NAME} ${INCLUDE_FILES} ${SRC_FILES})

        source_group(
            TREE ${LIBRARY_PATH}
            PREFIX ""
            FILES ${INCLUDE_FILES} ${SRC_FILES}
        )

        target_include_directories(
            ${PROJECT_NAME} PUBLIC
            ${LIBRARY_PATH}/include
            ${LIBRARY_PATH}/source
        )

        target_compile_options(
            ${PROJECT_NAME} PRIVATE
            /W0
        )

        set_source_files_properties(
            ${INCLUDE_FILES}
            PROPERTIES HEADER_FILE_ONLY TRUE
        )
    endif()
endforeach()
