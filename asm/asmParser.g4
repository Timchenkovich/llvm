parser grammar asmParser;

options {
	tokenVocab = asmLexer;
}

asm: line+;

line: function | label | instruction;

function: FUN ID COLON;
label: ID COLON;
instruction: add | branch | call;

regOrImm: REG | MINUS? IMM;

// instruction definitions

add: ADD REG regOrImm regOrImm;
branch:
	CJ REG ID
	| INCJNEQ REG IMM ID
	| JNPOS REG ID
	| JMP ID
	| RET;

// call doesn't really change control flow
call: CALL ID;