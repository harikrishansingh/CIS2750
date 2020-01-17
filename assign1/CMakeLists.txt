cmake_minimum_required(VERSION 3.7.10)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project("2750A1")
find_package(LibXml2 REQUIRED)

set(CMAKE_C_FLAGS "-Wall -g -std=c11")
set(CMAKE_BINARY_DIR bin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${LIBXML2_INCLUDE_DIR})

add_library(svgparse SHARED src/SVGParser.c)
target_link_libraries(svgparse PUBLIC ${LIBXML2_LIBRARIES})

add_executable(test src/main.c)
target_link_libraries(test svgparse)
