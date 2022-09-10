cmake_minimum_required(VERSION 3.16)

SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
SET(GRID_VERSION_MAX 1.2)
# string(APPEND GRID_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

project(grid)

file(GLOB_RECURSE MAIN
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/grid/src/*.cpp 
         ${CMAKE_CURRENT_SOURCE_DIR}/grid/src/*.h)

file(GLOB_RECURSE ASSETS  
    ${CMAKE_CURRENT_SOURCE_DIR}/grid/assets/shaders/*
)

source_group("assets" FILES ${ASSETS})

option(USE_CORE "Use zar" ON)
if(USE_CORE)
    add_subdirectory(zar)
    list(APPEND EXTRA_LIBS zar)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/zar")
endif()

# -- GENERAMOS EL ARCHIVO *.h EN EL QUE TENDREMOS LOS VALORES DE LAS VARIABLES
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)

find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_library(OPENVR_LIBRARY openvr_api)

# physx library
find_library(PHYSX1_LIBRARY LowLevel_static_64)
find_library(PHYSX2_LIBRARY LowLevelAABB_static_64)
find_library(PHYSX3_LIBRARY LowLevelDynamics_static_64)
find_library(PHYSX4_LIBRARY PhysX_64)
find_library(PHYSX5_LIBRARY PhysXCharacterKinematic_static_64)
find_library(PHYSX6_LIBRARY PhysXCommon_64)
find_library(PHYSX7_LIBRARY PhysXCooking_64)
find_library(PHYSX8_LIBRARY PhysXExtensions_static_64)
find_library(PHYSX9_LIBRARY PhysXFoundation_64)
find_library(PHYSX10_LIBRARY PhysXPvdSDK_static_64)
find_library(PHYSX11_LIBRARY PhysXTask_static_64)
find_library(PHYSX12_LIBRARY PhysXVehicle_static_64)
find_library(PHYSX13_LIBRARY SceneQuery_static_64)
find_library(PHYSX14_LIBRARY SimulationController_static_64)


include_directories(zar/vendor/glm)
include_directories(zar/vendor/stb)

add_executable(${PROJECT_NAME} ${MAIN})

target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imguizmo::imguizmo)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENVR_LIBRARY})

# physx link
target_link_libraries(${PROJECT_NAME} PUBLIC ${PHYSX1_LIBRARY} ${PHYSX2_LIBRARY} 
${PHYSX3_LIBRARY} ${PHYSX4_LIBRARY} ${PHYSX5_LIBRARY} ${PHYSX6_LIBRARY} ${PHYSX7_LIBRARY} 
${PHYSX8_LIBRARY} ${PHYSX9_LIBRARY} ${PHYSX10_LIBRARY} ${PHYSX11_LIBRARY} ${PHYSX12_LIBRARY} 
${PHYSX13_LIBRARY} ${PHYSX14_LIBRARY})

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

                
target_sources(${PROJECT_NAME} PRIVATE ${ASSETS})
