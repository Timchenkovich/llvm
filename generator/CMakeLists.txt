project(compilers)

set(CMAKE_BUILD_TYPE Debug)

add_executable(executorGenerator executor_generator.cpp)
add_dependencies(executorGenerator makeLL)

add_custom_command(
    OUTPUT executor.cpp
    COMMAND executorGenerator ${APP_LL_PATH} executor.cpp 
    DEPENDS executorGenerator ${APP_LL_PATH}
)

add_custom_target(makeExecutor DEPENDS executor.cpp)

include_directories(${LLVM_INCLUDE_DIRS} ../sim)

add_executable(executor executor.cpp)
add_dependencies(executor makeExecutor)


llvm_map_components_to_libnames(llvm_libs core executionengine ${LLVM_TARGETS_TO_BUILD})
target_link_libraries(executor ${llvm_libs} sim)

add_custom_command(
    OUTPUT generated.ll
    COMMAND executor --dump
    DEPENDS executor
)

add_custom_target(genGeneratedLL DEPENDS generated.ll)


if (WIN32)
    set (LL_COMPILER ${CMAKE_CXX_COMPILER})
else()
    set (LL_COMPILER "llc")
endif()

add_custom_command(
    OUTPUT generatedApp
    COMMAND ${LL_COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/generated.ll -o compiledLL.s -mtriple=${LLVM_HOST_TRIPLE}
    && ${CMAKE_CXX_COMPILER} compiledLL.s -c -o generatedApp -target ${LLVM_HOST_TRIPLE}
    DEPENDS genGeneratedLL generated.ll
)

add_custom_target(buildGeneratedApp DEPENDS generatedApp)

add_library(generatedAppLib STATIC IMPORTED)
add_dependencies(generatedAppLib buildGeneratedApp)
set_target_properties(generatedAppLib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/generatedApp)

add_executable(startWithGeneratedApp ${START_SOURCES})
target_link_libraries(startWithGeneratedApp generatedAppLib sim SDL2-static)
