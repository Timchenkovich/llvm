lexer grammar asmLexer;

import asmInstructionLexer;

COLON: ':';
FUN: 'fun';
MINUS: '-';
COMMA: ',';

// instruction tokens
ADD: 'add';
SUB: 'sub';
SHL: 'shl';
CMPGT: 'cmpgt';
CMPGTE: 'cmpgte';
CMPLT: 'cmplt';
CMPNEQ: 'cmpneq';
OR: 'or';
AND: 'and';
SLCTEV: 'slctev';
MUL: 'mul';
MULADD: 'muladd';
MOV: 'mov';
SELECT: 'select';
FLUSH: 'flush';
RND: 'rnd';
PUTPXL: 'putpxl';
RNDRNG: 'rndrng';

// control flow instructions
JMP: 'jmp';
CJ: 'cj';
INCJNEQ: 'incjneq';
JNPOS: 'jnpos';
CALL: 'call';
RET: 'ret';

IMM: [0-9]+;
REG: 'x' [0-9]+;
ID: [a-zA-Z_] [0-9a-zA-Z_]*;
WS: [ \t\n\r\f]+ -> skip;
COMMENT: ';' .*? ('\r'? '\n') -> skip;