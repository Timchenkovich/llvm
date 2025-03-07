project(compilers)

message("asm include dirs: ${ASM_INCLUDE_DIRS}")
include_directories(${ASM_INCLUDE_DIRS})

add_library(instructionCreatorUtils utils.cpp)
target_link_libraries(instructionCreatorUtils asmSources)
