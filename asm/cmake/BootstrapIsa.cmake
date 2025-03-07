macro(bootstrap_isa isaPathStr)
    cmake_path(SET isaPath NORMALIZE ${isaPathStr})
    cmake_path(GET isaPath STEM isaName)

    get_filename_component(isaName ${isaPath} NAME_WLE)
    
    set(outputDir "${isaName}_generated")
    
    set(GENERATED_SOURCES_NAMES 
    "emulatingInstructionCreator.h" 
    "emulatingInstructionCreator.cpp"
    "emulator.h"
    "emulator.cpp"
    )


    foreach(item IN LISTS GENERATED_SOURCES_NAMES)
    list(APPEND ${isaName}_GENERATED_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${outputDir}/${item}")
    endforeach()


    set(${isaName}_INSTRUCTION_LEXER "${CMAKE_CURRENT_BINARY_DIR}/${outputDir}/${isaName}InstructionLexer.g4")
    set(${isaName}_INSTRUCTION_PARSER "${CMAKE_CURRENT_BINARY_DIR}/${outputDir}/${isaName}InstructionParser.g4")
    set(${isaName}_GRAMMAR_SOURCES ${${isaName}_INSTRUCTION_PARSER} ${${isaName}_INSTRUCTION_LEXER})


    add_custom_command(
    OUTPUT ${${isaName}_GENERATED_SOURCES} ${${isaName}_GRAMMAR_SOURCES}
    COMMAND mkdir -p ${outputDir} && cd ${outputDir} && ${CMAKE_BINARY_DIR}/asm/isa-creator/bootstrapIsa ${isaPath}
    DEPENDS ${CMAKE_BINARY_DIR}/asm/isa-creator/bootstrapIsa ${isaPath})

    add_custom_target(generateInstructionGrammar DEPENDS ${${isaName}_GRAMMAR_SOURCES})

    set(${isaName}_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/${outputDir})
    set(${isaName}_ISA_SOURCES ${${isaName}_GENERATED_SOURCES})
endmacro()