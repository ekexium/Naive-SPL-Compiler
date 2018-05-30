%{
#include <string>
#include "node.h"
#include "parser.hpp"

#define SAVE_TOKEN  yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
%}
ID [a-zA-Z][_a-zA-Z0-9]*
LP "("
RP ")"
LB "["
RB "]"
DOT "."
COMMA ","
COLON ":"
MUL "*"
DIV "/"
UNEQUAL "<>"
NOT "NOT"
PLUS "+"
MINUS "-"
GE ">="
GT ">"
LE "<="
LT "<"
EQUAL "="
ASSIGN ":="
MOD "MOD"
DOTDOT ".."
SEMI ";"
SYS_CON "false" | "true" | "maxint"
SYS_FUNCT "abs" | "chr" | "odd" | "pred" | "sqr" | "sqrt" | "succ"
SYS_PROC "write" | "writeln"
SYS_TYPE "boolean" | "char" | "integer" | "real"
%%
[ \t\n] ;
{name} { yylval = strdup(yytext);
return NAME; }
{eq} { return EQ; }
{age} { yylval = strdup(yytext);
return AGE; }
%%
int yywrap()
{
return 1;
}