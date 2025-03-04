lexer grammar asmLexer;

COLON: ':';
FUN: 'fun';
MINUS: '-';

// instruction tokens
ADD: 'add';

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