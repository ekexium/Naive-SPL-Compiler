#include <iostream>
#include <stdio.h>
#include "AST.h"
#include "codegen.h"
#include "parser.hpp"
extern Program *astRoot;

extern int yyparse();

int main(int argc, char **argv)
{
	std::string sourceFile = "/Users/qzq/code/compiler/splc/test/test1.spl";
	extern FILE * yyin;
	yyin=fopen(sourceFile.c_str(),"r");
	yyparse();
	std::cout << astRoot << std::endl;
	CodeGenContext context;
	context.generateCode(astRoot);
	return 0;
}