project(compilers)

# add external build for antlrcpp
include(ExternalAntlr4Cpp)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(BootstrapIsa)

bootstrap_isa(${CMAKE_CURRENT_SOURCE_DIR}/asm.isa)
set(ASM_INCLUDE_DIRS ${asm_GENERATED_DIR})

include_directories(
    module-context 
    ${ANTLR4_INCLUDE_DIRS} 
    func-creator
    source-descriptor
    ${SIM_PATH}
    ${ASM_INCLUDE_DIRS}
)

# once instruction grammars are generated, initialize antlr generator chain
antlr_target(
    IsaLexer
    ${asm_INSTRUCTION_LEXER}
    LEXER
    OUTPUT_DIRECTORY ${asm_GENERATED_DIR}
    DEPENDS generateInstructionGrammar ${asm_GRAMMAR_SOURCES} 
)

antlr_target(
    AsmLexer
    ${CMAKE_CURRENT_SOURCE_DIR}/asmLexer.g4
    LEXER
    DEPENDS_ANTLR IsaLexer
    OUTPUT_DIRECTORY ${asm_GENERATED_DIR}
    COMPILE_FLAGS -lib ${asm_GENERATED_DIR}
)


antlr_target( 
    IsaParser
    ${asm_INSTRUCTION_PARSER}
    PARSER
    DEPENDS_ANTLR AsmLexer
    OUTPUT_DIRECTORY ${asm_GENERATED_DIR}
    COMPILE_FLAGS -lib ${asm_GENERATED_DIR}
)

antlr_target(
    AsmParser
    ${CMAKE_CURRENT_SOURCE_DIR}/asmParser.g4
    PARSER
    LISTENER
    VISITOR
    DEPENDS_ANTLR IsaParser
    COMPILE_FLAGS -lib ${asm_GENERATED_DIR}
    OUTPUT_DIRECTORY ${asm_GENERATED_DIR}
)


add_library(asmSources ${ANTLR_AsmParser_CXX_OUTPUTS}
${ANTLR_AsmLexer_CXX_OUTPUTS} )
target_link_libraries(asmSources antlr4_static)



add_subdirectory(isa-creator)
add_subdirectory(module-context)
add_subdirectory(source-descriptor)

add_subdirectory(func-creator)
add_subdirectory(instruction-creator)



add_library(emulatorGenerated ${asm_ISA_SOURCES})

llvm_map_components_to_libnames(llvm_libs core executionengine mcjit interpreter ${LLVM_TARGETS_TO_BUILD})
# this will also bring includes
target_link_libraries(emulatorGenerated instructionCreator ${llvm_libs})

add_library(emulatorImpl emulatorImpl.cpp)
target_link_libraries(emulatorImpl emulatorGenerated)

add_executable(lift lift.cpp)
target_link_libraries(lift 
moduleContext 
${llvm_libs} 
asmSources 
funcCreator 
instructionCreator 
cxxopts 
sim 
emulatorGenerated 
emulatorImpl)