project(compilers)

include_directories(
    ${ASM_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS} 
    ../bb-collector 
    ${CMAKE_SOURCE_DIR}/source-creator 
    ../source-descriptor
    ../module-context
)
llvm_map_components_to_libnames(llvm_libs core)

add_library(funcCreator funcCreator.cpp)
target_link_libraries(funcCreator asmSources ${llvm_libs} bbCollector sourceDescriptor moduleContext)
