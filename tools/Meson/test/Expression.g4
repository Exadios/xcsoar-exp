grammar Expression;
  
// Parser rule
expr    : '-' expr                  #UnariyMinus
        | expr ( '*' | '/' ) expr   #Multiplacative
        | expr ( '+' | '-' ) expr   #Additivive
        | '(' expr ')'              #Parenthsis 
        | INT                       #Int 
        | ID                        #Id
        ;                     
  
// Lexer rules
INT     : [0-9]+;
ID      : [a-z]+;
WS      : [ \t\r\n]+ -> skip;
