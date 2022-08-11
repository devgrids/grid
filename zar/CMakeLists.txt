cmake_minimum_required(VERSION 3.16)
project(zar)

file(GLOB_RECURSE DLL
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/zar/*.cpp 
         ${CMAKE_CURRENT_SOURCE_DIR}/zar/*.h)
         
add_library(${PROJECT_NAME} SHARED ${DLL})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

target_precompile_headers(${PROJECT_NAME} PRIVATE 
"$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/zar/pch.h>")

message("OUTPUT: " ${PROJECT_SOURCE_DIR})