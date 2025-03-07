parser grammar isaParser;

options {
	tokenVocab = isaLexer;
}

isa: instruction*;

instruction: INSTR_NAME op*;
op: REG | IMM | RIM;