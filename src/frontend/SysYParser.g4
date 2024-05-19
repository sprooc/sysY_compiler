parser grammar SysYParser;

options {
    tokenVocab = SysYLexer;
}

program
   : compUnit
   ;
compUnit
   : (funcDef | decl)+ EOF
   ;


decl
   : constDecl | varDecl
   ;

constDecl
   : CONST bType constDef ( COMMA constDef )* SEMICOLON
   ;

bType
   : INT
   ;

constDef
   : IDENT ( L_BRACKT constExp R_BRACKT )* ASSIGN constInitVal
   ;

constInitVal
   : constExp
   | L_BRACE ( constInitVal ( COMMA constInitVal )* )? R_BRACE
   ;

varDecl
   : bType varDef ( COMMA varDef )* SEMICOLON
   ;

varDef
   : IDENT ( L_BRACKT constExp R_BRACKT )*
   | IDENT ( L_BRACKT constExp R_BRACKT )* ASSIGN initVal
   ;

initVal
   : exp 
   | L_BRACE ( initVal ( COMMA initVal )* )? R_BRACE
   ;

funcDef 
   : funcType funcName L_PAREN (funcFParams)? R_PAREN block
   ;

funcType 
   : VOID 
   | INT
   ;

funcFParams 
   : funcFParam ( COMMA funcFParam )*
   ;

funcFParam 
   : bType IDENT (L_BRACKT R_BRACKT ( L_BRACKT exp R_BRACKT )* )?
   ;

block 
   : L_BRACE ( blockItem )* R_BRACE
   ;

blockItem
   : decl 
   | stmt
   ;

stmt 
   : lVal ASSIGN exp SEMICOLON 
   | (exp)? SEMICOLON 
   | block
   | ifElseStmt
   | whileStmt
   | BREAK SEMICOLON 
   | CONTINUE SEMICOLON
   | RETURN (exp)? SEMICOLON
   ;

ifElseStmt
   : IF L_PAREN cond R_PAREN stmt ( ELSE stmt )?
   ;

whileStmt
   : WHILE L_PAREN cond R_PAREN stmt
   ;


exp
  : lOrExp
  ;

cond
  : lOrExp
  ;

lVal
  : IDENT (L_BRACKT exp R_BRACKT)*
  ;

primaryExp
  : L_PAREN exp R_PAREN
  | lVal
  | number
  ;


number
   : INTEGER_CONST
   | FLOAT_CONST
   ;

unaryExp
  : primaryExp
  | funcName L_PAREN funcRParams? R_PAREN 
  | unaryOp unaryExp
  ;

unaryOp
  : PLUS
  | MINUS
  | NOT
  ;

funcRParams
   : exp (COMMA exp)*
   ;

mulExp
  : unaryExp
  | mulExp (MUL | DIV | MOD) unaryExp
  ;

addExp
  : mulExp
  | addExp (PLUS | MINUS) mulExp
  ;

relExp
  : addExp
  | relExp (LT | GT | LE | GE) addExp
  ;

eqExp
  : relExp
  | eqExp (EQ | NEQ) relExp
  ;

lAndExp
  : eqExp
  | lAndExp AND eqExp
  ;

lOrExp
  : lAndExp
  | lOrExp OR lAndExp
  ;

constExp
  : exp
  ;

funcName
   : IDENT
   ;
   


