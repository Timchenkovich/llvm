project(compilers)


include_directories(${ASM_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(llvm_libs core)

add_library(moduleContext moduleContext.cpp)
target_link_libraries(moduleContext ${llvm_libs})