/*
 * Parser for make -np TARGET=<target> output
 */

/* Parser rules */

grammar Make;

partial_var : NAME {System.out.println("partial_var");};

/* Lexer rules */

NAME   : [A-Z][A-Z_]* ;
