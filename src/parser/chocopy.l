%option noyywrap
%option yylineno

/* The prefix for imports in c syntax */
%{
/* For Indentation */
#include <stack>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#if __cplusplus > 199711L
#define register      // Deprecated in C++11.
#endif  // #if __cplusplus > 199711L

using namespace std;
std::stack<int> stack_indent;

#include <chocopy_parse.hpp>
#include "chocopy.tab.h"

int e1 = 1, e2 = 1,global_leng=0;
string info = "";

#define LITERAL(type) { \
    LOG(DEBUG) << #type << " " <<  yytext << " location: "<< yylloc.first_line<<" " << yylloc.first_column<< " " << (e1 = yylloc.last_line) <<" " << (e2 = yylloc.last_column); \
    return TOKEN_##type; \
}

#define LITERAL_END(type) { \
    LOG(DEBUG) << #type << " " <<  yytext << " location: "<< yylloc.first_line<<" " << yylloc.first_column<< " " << (e1 = yylloc.last_line) <<" " << (e2 = yylloc.last_column); \
    yylloc.last_column=1; yylloc.last_line+=1;  \
    return TOKEN_##type; \
}

int ColNum = 1;
int LineNum = 1;
#define SET_ACTION                                                           \
  yylloc.first_line = LineNum;                                               \
  yylloc.first_column = ColNum;                                              \
  ColNum += yyleng;                                                          \
  yylloc.last_line = LineNum;                                                \
  yylloc.last_column = ColNum;

#define ANOTHER_LINE                                                         \
  ColNum = 1;                                                                \
  LineNum += 1;
#define CLEAR_ONE                                                         \
  ColNum -= 4;                                                                \

#define YY_ANOTHER_TAB                                                       \
  ColNum += 4;
#define YY_ANOTHER_SPACE                                                     \
  ColNum += 1;
#define YY_ANOTHER_STR(i)                                                    \
  ColNum += (i);
%}

%x STRING
%x REALLYEND

%%

%{
  /* Init indent stack_indent */
  if (stack_indent.empty()) {
    stack_indent.push(0);
  }

/* The Lexical rules Your Code Below */
%}

.   {
        LOG(ERROR) << "Error: Unrecognized token on line " << yylineno << ": " << yytext;
        return 1;
    }
%%

