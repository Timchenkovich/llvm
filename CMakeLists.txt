cmake_minimum_required(VERSION 3.20.0)
project(compilers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(FetchContent)

FetchContent_Declare(
    sdl
    GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
    GIT_TAG SDL2
)

FetchContent_MakeAvailable(sdl)

message("compiler ${CMAKE_CXX_COMPILER_ID}")
if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang") 
    message(FATAL_ERROR "Clang compiler expected to be used")
endif()

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})


message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

message("Target cpu: ${CMAKE_SYSTEM_PROCESSOR}")
message("Host cpu: ${CMAKE_HOST_SYSTEM_PROCESSOR}")


add_subdirectory(sim)
add_subdirectory(app)
add_subdirectory(generator)
add_subdirectory(analysis)
