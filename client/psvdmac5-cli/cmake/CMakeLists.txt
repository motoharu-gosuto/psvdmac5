cmake_minimum_required (VERSION 3.0)

set (PROJECT psvdmac5-cli)
project (${PROJECT})

FILE (GLOB HEADER_FILES "")
source_group ("Header Files" FILES ${HEADER_FILES})

FILE (GLOB SOURCE_FILES "../psvdmac5-cli.cpp")
source_group ("Source Files" FILES ${SOURCE_FILES})

add_executable(${PROJECT} ${HEADER_FILES} ${SOURCE_FILES})
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/output")

target_link_libraries (${PROJECT} "$ENV{PSVDMAC5_LIBRARY_DIR}\\psvdmac5-cli-lib.lib")

include_directories(../)
include_directories("$ENV{PSVDMAC5_INCLUDE_DIR}")
