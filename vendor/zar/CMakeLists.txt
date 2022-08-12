cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE DLL
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/zar/*.cpp 
         ${CMAKE_CURRENT_SOURCE_DIR}/zar/*.h)

add_library(zar SHARED ${DLL})

target_precompile_headers(zar PRIVATE 
"$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/zar/pch.h>")