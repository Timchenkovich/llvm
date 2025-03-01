project(compilers)

add_executable(app app.c)
target_link_libraries(app start)

set(APP_LL_PATH ${CMAKE_CURRENT_BINARY_DIR}/app.ll PARENT_SCOPE)


add_custom_command(
    OUTPUT app.ll
    COMMAND ${CMAKE_C_COMPILER} -emit-llvm -S -O2 ${CMAKE_CURRENT_SOURCE_DIR}/app.c -o app.ll
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/app.c)
add_custom_target(makeLL DEPENDS app.ll)


