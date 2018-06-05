%{
#include <string>
#include "AST.h"
#include "parser.hpp"

#define  SaveToken  yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (printf("%s\n", yytext), yylval.token = t)
%}

sys_con "false" | "true" | "maxint"
sys_funct "abs" | "chr" | "odd" | "pred" | "sqr" | "sqrt" | "succ"
sys_proc "write" | "writeln"
sys_type "boolean" | "char" | "integer" | "real"


%%
[ \t\n] ;
"read" return TOKEN(READ);
sys_con  SaveToken; return SYS_CON;
sys_funct  SaveToken; return SYS_FUNCT;
sys_proc SaveToken; return SYS_PROC;
sys_type  SaveToken; return SYS_TYPE;
"("  return TOKEN(LP);
")"  return TOKEN(RP);
"["  return TOKEN(LB);
"]"  return TOKEN(RB);
"."  return TOKEN(DOT);
","  return TOKEN(COMMA);
":"  return TOKEN(COLON);
"*"  return TOKEN(MUL);
"/"  return TOKEN(DIV);
"<>"  return TOKEN(NE);
"NOT"  return TOKEN(NOT);
"+"  return TOKEN(PLUS);
"-"  return TOKEN(MINUS);
">="  return TOKEN(GE);
">"  return TOKEN(GT);
"<="  return TOKEN(LE);
"<"  return TOKEN(LT);
"="  return TOKEN(EQ);
":="  return TOKEN(ASSIGN);
"MOD"  return TOKEN(MOD);
"%" return TOKEN(MOD);
".."  return TOKEN(DOTDOT);
";"  return TOKEN(SEMI);
"and"  return TOKEN(AND);
"array"  return TOKEN(ARRAY);
"begin"  return TOKEN(BBEGIN);
"case"  return TOKEN(CASE);
"const"  return TOKEN(CONST);
"div"  return TOKEN(DIV);
"do"  return TOKEN(DO);
"downto"  return TOKEN(DOWNTO);
"else"  return TOKEN(ELSE);
"end"  return TOKEN(END);
"for"  return TOKEN(FOR);
"function"  return TOKEN(FUNCTION);
"goto"  return TOKEN(GOTO);
"if"  return TOKEN(IF);
"in"  return TOKEN(IN);
"mod"  return TOKEN(MOD);
"not"  return TOKEN(NOT);
"of"  return TOKEN(OF);
"or"  return TOKEN(OR);
"packed"  return TOKEN(PACKED);
"procedure"  return TOKEN(PROCEDURE);
"program"  return TOKEN(PROGRAM);
"record"  return TOKEN(RECORD);
"repeat"  return TOKEN(REPEAT);
"set"  return TOKEN(SET);
"then"  return TOKEN(THEN);
"to"  return TOKEN(TO);
"type"  return TOKEN(TYPE);
"until"  return TOKEN(UNTIL);
"var"  return TOKEN(VAR);
"while"  return TOKEN(WHILE);
"with"  return TOKEN(WITH);
[a-zA-Z][_a-zA-Z0-9]* SaveToken; return ID;
[0-9]+  SaveToken; return INTEGER;
[0-9]+\.[0-9]* SaveToken; return REAL;
'^'' SaveToken; return CHAR;
'[^']*' SaveToken; return STRING;
. printf("Unknown token:%s\n", yytext); yyterminate();
%%
int yywrap()
{
return 1;
}
