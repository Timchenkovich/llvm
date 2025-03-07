parser grammar asmParser;

import asmInstructionParser;

options {
	tokenVocab = asmLexer;
}

asm: line+;

line: function | label | instruction;

function: FUN ID COLON;
label: ID COLON;
instruction: branch | call | simpleInstruction;

// regOrImm: REG | MINUS? IMM;

// instruction definitions

// add: ADD REG COMMA regOrImm COMMA regOrImm; sub: SUB REG COMMA regOrImm COMMA regOrImm; shl: SHL
// REG COMMA REG COMMA IMM; cmpgt: CMPGT REG COMMA regOrImm COMMA regOrImm; cmpgte: CMPGTE REG COMMA
// regOrImm COMMA regOrImm; cmplt: CMPLT REG COMMA regOrImm COMMA regOrImm; cmpneq: CMPNEQ REG COMMA
// regOrImm COMMA regOrImm; or: OR REG COMMA regOrImm COMMA regOrImm; and: AND REG COMMA regOrImm
// COMMA regOrImm; slctev: SLCTEV REG COMMA REG COMMA regOrImm COMMA regOrImm; mul: MUL REG COMMA
// regOrImm COMMA regOrImm; muladd: MULADD REG COMMA REG COMMA regOrImm COMMA regOrImm; mov: MOV REG
// COMMA regOrImm; select: SELECT REG COMMA REG COMMA regOrImm COMMA regOrImm; flush: FLUSH; rnd:
// RND REG; putpxl: PUTPXL regOrImm COMMA regOrImm COMMA regOrImm; rndrng: RNDRNG REG COMMA IMM;

// control flow instructions
branch:
	CJ REG COMMA ID
	| INCJNEQ REG COMMA IMM COMMA ID
	| JNPOS REG COMMA ID
	| JMP ID
	| RET;

// call doesn't really change control flow
call: CALL ID;