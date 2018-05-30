%{
#include <string>
#include "node.h"
#include "parser.hpp"

#define  SaveToken  yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
%}

sys_con "false" | "true" | "maxint"
sys_funct "abs" | "chr" | "odd" | "pred" | "sqr" | "sqrt" | "succ"
sys_proc "write" | "writeln"
sys_type "boolean" | "char" | "integer" | "real"


%%
[ \t\n] ;
sys_con  SaveToken; return TOKEN(SYS_CON);
sys_funct  SaveToken; return TOKEN(SYS_FUNCT);
sys_proc SaveToken; return TOKEN(SYS_PROC);
sys_type  SaveToken; return TOKEN(SYS_TYPE);
"(" SaveToken; return LP;
")" SaveToken; return RP;
"[" SaveToken; return LB;
"]" SaveToken; return RB;
"." SaveToken; return DOT;
"," SaveToken; return COMMA;
":" SaveToken; return COLON;
"*" SaveToken; return MUL;
"/" SaveToken; return DIV;
"<>" SaveToken; return NE;
"NOT" SaveToken; return NOT;
"+" SaveToken; return PLUS;
"-" SaveToken; return MINUS;
">=" SaveToken; return GE;
">" SaveToken; return GT;
"<=" SaveToken; return LE;
"<" SaveToken; return LT;
"=" SaveToken; return EQ;
":=" SaveToken; return ASSIGN;
"MOD" SaveToken; return MOD;
".." SaveToken; return DOTDOT;
";" SaveToken; return SEMI;
"and" SaveToken; return AND;
"array" SaveToken; return ARRAY;
"begin" SaveToken; return BEGIN;
"case" SaveToken; return CASE;
"const" SaveToken; return CONST;
"div" SaveToken; return DIV;
"do" SaveToken; return DO;
"downto" SaveToken; return DOWNTO;
"else" SaveToken; return ELEE;
"end" SaveToken; return END;
"for" SaveToken; return FOR;
"function" SaveToken; return FUNCTION;
"goto" SaveToken; return GOTO;
"if" SaveToken; return IF;
"in" SaveToken; return IN;
"mod" SaveToken; return MOD;
"not" SaveToken; return NOT;
"of" SaveToken; return OF;
"or" SaveToken; return OR;
"packed" SaveToken; return PACKED;
"procedure" SaveToken; return PROCEDURE;
"program" SaveToken; return PROGRAM;
"record" SaveToken; return RECORD;
"repeat" SaveToken; return REPEAT;
"set" SaveToken; return SET;
"then" SaveToken; return THEN;
"to" SaveToken; return TO;
"type" SaveToken; return TYPE;
"until" SaveToken; return UNTIL;
"var" SaveToken; return VAR;
"while" SaveToken; return WHILE;
"with" SaveToken; return WITH;
[a-zA-Z][_a-zA-Z0-9]* SaveToken; return ID;
. printf("Unknown token!/n"); yyterminate();
%%
int yywrap()
{
return 1;
}