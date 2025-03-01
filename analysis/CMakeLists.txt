project(compilers)

add_custom_command(
    OUTPUT logPass
    COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/pass.cpp -std=c++17 -fPIC -shared $(shell llvm-config --cppflags --ldflags --libs) -o libPass
    DEPENDS pass.cpp
)

add_custom_target(makeLogPass DEPENDS logPass)


add_custom_command(
    OUTPUT appWithPass
    COMMAND ${CMAKE_CXX_COMPILER} -fpass-plugin=${CMAKE_CURRENT_BINARY_DIR}/logPass -c -O2 
    DEPENDS logPass makeLogPass
)


