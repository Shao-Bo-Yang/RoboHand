cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(COMPILE_OPTIONS
    -Wno-unused-parameter
    -Wno-unused-includes

    # -fexceptions
    -D_LIBCPP_HAS_NO_THREADS=1
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND COMPILE_OPTIONS -O3)
endif()

file(GLOB SRCS
    ./BSD/*.cc
    ./BSD/*.c
    ./Base/*.c
    ./Base/*.cc
    ./Run/**.c
    ./Run/**.cc
    ./Run/command_tasks/*.cc
)
execute_process(COMMAND rm STM32F427IIHx_FLASH.ld)
execute_process(COMMAND cp STM32F427IIHx_FLASH_BK_.ld STM32F427IIHx_FLASH.ld)

# Set the project name
set(CMAKE_PROJECT_NAME RoboHand)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user sources here
    ${SRCS}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user defined include paths
    ./BSD
    ./Base
    ./Run
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    ${COMPILE_OPTIONS}
)

message(NOTICE "Build Dir: " ${CMAKE_BINARY_DIR})
install(CODE "execute_process(COMMAND /bin/sh ./install.sh ${CMAKE_BINARY_DIR} ${CMAKE_PROJECT_NAME})")
