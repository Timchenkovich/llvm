project(compilers)

include_directories(.. ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(llvm_libs core)

add_library(emulatingInstructionCreator emulatingInstructionCreator.cpp)
target_link_libraries(emulatingInstructionCreator PRIVATE branchInstructionCreator ${llvm_libs})