project(compilers)

message("func creator asm include dir: ${ASM_INCLUDE_DIRS}")
include_directories(
    ${ASM_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/source-creator 
    ../source-descriptor
    ../module-context
)
llvm_map_components_to_libnames(llvm_libs core)

add_library(funcCreator funcCreator.cpp)
target_link_libraries(funcCreator asmSources ${llvm_libs} sourceDescriptor moduleContext)
