project(compilers)

include_directories(${ANTLR4_INCLUDE_DIRS})

antlr_target(
    IsaLexer
    ${CMAKE_CURRENT_SOURCE_DIR}/isaLexer.g4
    LEXER
)

antlr_target(
    IsaParser
    ${CMAKE_CURRENT_SOURCE_DIR}/isaParser.g4
    PARSER
    LISTENER
    VISITOR
    DEPENDS_ANTLR IsaLexer
    COMPILE_FLAGS -lib ${ANTLR_IsaLexer_OUTPUT_DIR}
)

set(ISA_INCLUDE_DIRS ${ANTLR_IsaParser_OUTPUT_DIR} ${ANTLR_IsaLexer_OUTPUT_DIR})


add_library(isaSources ${ANTLR_IsaParser_CXX_OUTPUTS}
${ANTLR_IsaLexer_CXX_OUTPUTS} )
target_link_libraries(isaSources antlr4_static)


include_directories(${ISA_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/source-creator)

add_library(isaCreator isaCreator.cpp)
target_link_libraries(isaCreator sourceCreator isaSources)

add_executable(bootstrapIsa bootstrapIsa.cpp)
target_link_libraries(bootstrapIsa isaCreator isaSources antlr4_static)