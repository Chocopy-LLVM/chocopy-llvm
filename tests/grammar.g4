
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 victoryang00
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Project      : chocopy-compiler; an ANTLR4 grammar for ChocoPy
 *                https://github.com/victoryang00/chocopy-compiler
 */
grammar ChocoPy;

/*tokens { INDENT, DEDENT }*/

/*
 * parser rules
 */

program: (var_def | func_def | class_def)* stmt+;

class_def: 'class ' ID '(' ID ')' ' : ' NEWLINE ' INDENT ' class_body ' DEDENT ';

class_body: 'pass ' NEWLINE | (var_def |func_def)+;

func_def: 'def ' ID '(' (typed_var (',' typed_var )* )? ')' ' -> ' type ' :' NEWLINE ' INDENT ' func_body 'DEDENT';

func_body: (global_decl | nonlocal_decl |var_def |func_def )* stmt+;

typed_var: ID ':' type;

type: ID | STRING ;

global_decl: 'global' ID NEWLINE;

nonlocal_decl: 'nonlocal' ID NEWLINE;

var_def: typed_var '=' literal NEWLINE;

stmt: simple_stmt NEWLINE 
    | 'if ' expr ':' block ('elif ' expr ' :' block)* ( 'else :' block)?
    | 'while ' expr ' :' block 
    | 'for ' ID ' in ' expr ' : ' block;

simple_stmt: 'pass'
           | expr 
           | 'return ' (expr)?
           | target '=' expr; 

block: NEWLINE ' INDENT ' stmt+ ' DEDENT ';

literal: ' None ' | ' True ' | ' False ' | INTEGER | STRING;

expr: cexpr
    | ' not ' expr
    | expr (' and ' | ' or ') expr
    | expr ' if ' expr ' else ' expr;

cexpr: ID 
     | literal 
     | member_expr
     | index_expr
     | member_expr '(' (expr(',' expr)*)? ')'
     | ID '(' (expr (',' expr)*)? ')' 
     | cexpr bin_op cexpr 
     | '-' expr;

bin_op: '+' | '-' | '*' | '%' | '==' | '!=' | '<=' | '>=' | '<' | '>' | 'is';

member_expr: cexpr '.' ID;

index_expr: cexpr (expr);

target: ID
      | member_expr
      | index_expr;

/*
 * lexer rules
 */
STRING
 : STRING_LITERAL
 | BYTES_LITERAL
 ;
NUMBER
 : INTEGER
 | FLOAT_NUMBER
 | IMAG_NUMBER
 ;
INTEGER
 : DECIMAL_INTEGER
 | OCT_INTEGER
 | HEX_INTEGER
 | BIN_INTEGER
 ;
NEWLINE
 : ( '\n' );

ID
 : ID_START ID_CONTINUE*
 ;
STRING_LITERAL
 : ( [rR] | [uU] | [fF] | ( [fF] [rR] ) | ( [rR] [fF] ) )? ( SHORT_STRING )
 ;
BYTES_LITERAL
 : ( [bB] | ( [bB] [rR] ) | ( [rR] [bB] ) ) ( SHORT_BYTES | LONG_BYTES )
 ;

DECIMAL_INTEGER
 : NON_ZERO_DIGIT DIGIT*
 | '0'+
 ;

OCT_INTEGER
 : '0' [oO] OCT_DIGIT+
 ;

HEX_INTEGER
 : '0' [xX] HEX_DIGIT+
 ;

BIN_INTEGER
 : '0' [bB] BIN_DIGIT+
 ;

FLOAT_NUMBER
 : POINT_FLOAT
 | EXPONENT_FLOAT
 ;

IMAG_NUMBER
 : ( FLOAT_NUMBER | INT_PART ) [jJ]
 ;
UNKNOWN_CHAR
 : .
 ;
/* 
 * fragments 
 */

fragment SHORT_STRING
 : '\'' ( STRING_ESCAPE_SEQ | ~[\\\r\n\f']+ ) ~[\\\r\n\f']+ '\''
 ;

fragment STRING_ESCAPE_SEQ
 : '\\' .
 | '\\' NEWLINE
 ;

fragment NON_ZERO_DIGIT
 : [1-9]
 ;

fragment DIGIT
 : [0-9]
 ;

fragment OCT_DIGIT
 : [0-7]
 ;

fragment HEX_DIGIT
 : [0-9a-fA-F]
 ;

fragment BIN_DIGIT
 : [01]
 ;

fragment POINT_FLOAT
 : INT_PART? FRACTION
 | INT_PART '.'
 ;

fragment EXPONENT_FLOAT
 : ( INT_PART | POINT_FLOAT ) EXPONENT
 ;

fragment INT_PART
 : DIGIT+
 ;

fragment FRACTION
 : '.' DIGIT+
 ;

fragment EXPONENT
 : [eE] [+-]? DIGIT+
 ;


fragment SHORT_BYTES
 : '"' ( SHORT_BYTES_CHAR_NO_DOUBLE_QUOTE | BYTES_ESCAPE_SEQ )* '"'
 ;

fragment LONG_BYTES
 : '\'\'\'' LONG_BYTES_ITEM*? '\'\'\''
 | '"""' LONG_BYTES_ITEM*? '"""'
 ;

fragment LONG_BYTES_ITEM
 : LONG_BYTES_CHAR
 | BYTES_ESCAPE_SEQ
 ;

fragment SHORT_BYTES_CHAR_NO_SINGLE_QUOTE
 : [\u0000-\u0009]
 | [\u000B-\u000C]
 | [\u000E-\u0026]
 | [\u0028-\u005B]
 | [\u005D-\u007F]
 ; 
fragment SHORT_BYTES_CHAR_NO_DOUBLE_QUOTE
 : [\u0000-\u0009]
 | [\u000B-\u000C]
 | [\u000E-\u0021]
 | [\u0023-\u005B]
 | [\u005D-\u007F]
 ; 
fragment LONG_BYTES_CHAR
 : [\u0000-\u005B]
 | [\u005D-\u007F]
 ;
fragment BYTES_ESCAPE_SEQ
 : '\\' [\u0000-\u007F]
 ;
fragment SPACES
 : [  \t]+
 ;
fragment COMMENT
 : '#' ~[\r\n\f]*
 ;
fragment LINE_JOINING
 : '\\' SPACES? ( '\r'? '\n' | '\r' | '\f')
 ;
fragment ID_START
 : '_'
 | [A-Z]
 | [a-z]
 ;
fragment ID_CONTINUE
 : ID_START
 | [0-9]
 ;

