lexer grammar isaLexer;

REG: 'reg';
IMM: 'imm';
RIM: 'rim'; // reg or imm

INSTR_NAME: [a-z]+;
WS: [ \t\n\r] -> skip;