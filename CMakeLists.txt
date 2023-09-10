cmake_minimum_required(VERSION 3.20)
project(Cheap_8 VERSION 1.0.0 DESCRIPTION "Chip 8 Emulator")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

find_library(SDL2Fwk SDL2 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
		"${PROJECT_SOURCE_DIR}/src/*.c"
        )

add_executable(Cheap_8 ${all_SRCS})
target_link_libraries(Cheap_8 ${SDL2Fwk})