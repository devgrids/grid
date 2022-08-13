cmake_minimum_required(VERSION 3.16)

# SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
SET(GRID_VERSION_MAX 1.2)
# string(APPEND GRID_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

project(grid)

file(GLOB_RECURSE MAIN
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/grid/src/*.cpp 
         ${CMAKE_CURRENT_SOURCE_DIR}/grid/src/*.h)


option(USE_CORE "Use zar" ON)
if(USE_CORE)
    add_subdirectory(vendor/zar)
    list(APPEND EXTRA_LIBS zar)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/vendor/zar")
endif()


# -- GENERAMOS EL ARCHIVO *.h EN EL QUE TENDREMOS LOS VALORES DE LAS VARIABLES
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)

include_directories(vendor/zar/vendor/glm)
include_directories(vendor/zar/vendor/stb)

add_executable(${PROJECT_NAME} ${MAIN})

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})

target_precompile_headers(${PROJECT_NAME} PRIVATE 
"$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/grid/src/pch.h>")

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set_target_properties(zar PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

# message("OUTPUT: " ${PROJECT_SOURCE_DIR}/Core)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/assets
                ${CMAKE_CURRENT_BINARY_DIR}/assets)

