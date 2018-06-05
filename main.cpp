#include <iostream>
#include "AST.h"
extern Program *astRoot;

extern int yyparse();

int main(int argc, char **argv)
{
	std::string sourceFile = "/Users/qzq/code/compiler/splc/test/test1.spl";
	extern FILE * yyin;
	yyin=fopen(sourceFile.c_str(),"r");
	yyparse();
	std::cout << astRoot << std::endl;
	return 0;
}