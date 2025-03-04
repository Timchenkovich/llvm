project(compilers)

include_directories(../module-context)

add_library(sourceDescriptor sourceDescriptor.cpp)
target_link_libraries(sourceDescriptor moduleContext)
