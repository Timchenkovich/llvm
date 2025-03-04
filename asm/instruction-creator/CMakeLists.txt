project(compilers)

include_directories(${ASM_INCLUDE_DIRS} ../source-descriptor ../module-context utils)

add_library(branchInstructionCreator branchInstructionCreator.cpp)
target_link_libraries(branchInstructionCreator asmSources sourceCreator sourceDescriptor moduleContext instructionCreatorUtils)

add_subdirectory(utils)
add_subdirectory(emulating)
add_subdirectory(translating)

add_library(instructionCreator INTERFACE)
target_link_libraries(instructionCreator INTERFACE emulatingInstructionCreator translatingInstructionCreator)

