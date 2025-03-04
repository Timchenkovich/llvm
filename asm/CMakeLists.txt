project(compilers)

# add external build for antlrcpp
include(ExternalAntlr4Cpp)

include_directories(${ANTLR4_INCLUDE_DIRS})
message(${ANTLR4_INCLUDE_DIRS})

antlr_target(
    AsmLexer
    ${CMAKE_CURRENT_SOURCE_DIR}/asmLexer.g4
    LEXER
)

antlr_target(
    AsmParser
    ${CMAKE_CURRENT_SOURCE_DIR}/asmParser.g4
    PARSER
    LISTENER
    VISITOR
    DEPENDS_ANTLR AsmLexer
    COMPILE_FLAGS -lib ${ANTLR_AsmLexer_OUTPUT_DIR}
)

set(ASM_INCLUDE_DIRS ${ANTLR_AsmParser_OUTPUT_DIR} ${ANTLR_AsmLexer_OUTPUT_DIR})

include_directories(ASM_INCLUDE_DIRS)

add_library(asmSources ${ANTLR_AsmParser_CXX_OUTPUTS}
${ANTLR_AsmLexer_CXX_OUTPUTS} )
target_link_libraries(asmSources antlr4_static)


add_executable(liftAsm lift.cpp)
target_link_libraries(liftAsm asmSources)

add_subdirectory(module-context)
add_subdirectory(source-descriptor)

add_subdirectory(func-creator)
add_subdirectory(instruction-creator)