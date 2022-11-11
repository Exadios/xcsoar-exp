/*
 * Parser for make -np TARGET=<target> output
 */

/* Parser rules */

grammar Make;

project    : sentences;

sentences  : sentence
           | sentences sentence
           | EOF
           ;

sentence   : assignment   #typeAssignment
           ;

assignment : NAME EQ var EOL
           | NAME  ':' EQ  var EOL
           ;

var        : VAR
           | '$' '(' var ')'
           ;

/* Lexer rules */

NAME   : [A-Z][A-Z_]* ;
fragment Lower_case    : [a-z] ;
fragment Upper_case    : [A-Z] ;
fragment Digit         : [0-9.] ;
fragment Sq            : '\''  ;
fragment Dq            : '"'   ;
fragment Extra_char    : [_:\-//,<>%*^|@\\?;&{}!~ ()+.] ;
fragment Pound         : '#' ;
fragment Eol           : '\n' ;

CALL    : 'c' 'a' 'l' 'l' WS ;
TOPDIR  : 't' 'o' 'p' 'd' 'i' 'r' ;
VAR     : ( Upper_case) ( Upper_case | Lower_case | Extra_char )* ;

EQ      : '=';
EOL     : ('\r'? '\n' | '\r')+ ;
WS      : [ \t]+ ;
