project(compilers)

# include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${sdl_SOURCE_DIR}/include)
add_library(sim sim.c)
target_link_libraries(sim SDL2-static)

set(SIM_PATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(START_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/start.c PARENT_SCOPE)

add_library(start start.c)
target_link_libraries(start sim)
