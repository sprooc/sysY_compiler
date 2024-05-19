lexer grammar SysYLexer;

CONST : 'const' ;

INT : 'int' ;

VOID : 'void';

FLOAT : 'float';

IF : 'if';

ELSE : 'else';

WHILE : 'while';

BREAK : 'break';

CONTINUE : 'continue';

RETURN : 'return';

PLUS : '+';

MINUS : '-';

MUL : '*';

DIV : '/';

MOD : '%';

ASSIGN : '=';

EQ : '==';

NEQ : '!=';

LT : '<';

GT : '>';

LE : '<=';

GE : '>=';

NOT : '!';

AND : '&&';

OR : '||';

L_PAREN : '(';

R_PAREN : ')';

L_BRACE : '{';

R_BRACE : '}';

L_BRACKT : '[';

R_BRACKT : ']';

COMMA : ',';

SEMICOLON : ';';

IDENT : 
    [a-zA-Z_][a-zA-Z0-9_]*
   ;

INTEGER_CONST
   : [1-9][0-9]*
   | '0'
   | '0' [0-7]+
   | '0' [xX][0-9a-fA-F]+
   ;

FLOAT_CONST
	: [0-9]+ '.' [0-9]*
	| '.' [0-9]+ ([eE][+-]? [0-9]+)?
	| [0-9]+ ('.' [0-9]*)? [eE][+-]? [0-9]+
	| '0x' [0-9a-fA-F]+ '.' [0-9a-fA-F]*
	| '0x' '.' [0-9a-fA-F]+ ([pP][+-]? [0-9a-fA-F]+)?
	| '0x' [0-9a-fA-F]+ ('.' [0-9a-fA-F]*)? [pP][+-]? [0-9a-fA-F]+
  ;

WS
   : [ \r\n\t]+ -> skip
   ;

LINE_COMMENT
   : '//' .*? '\n' -> skip
   ;

MULTILINE_COMMENT
   : '/*' .*? '*/' -> skip
   ;