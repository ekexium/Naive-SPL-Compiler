//
// Created by QZQ on 2018/6/9.
//
#include "AST.h"
#include "parser.hpp"

extern int yyparse();

extern FILE *yyin;
extern Program *astRoot;

const int NUM_SYSTEM_FUNCTIONS = 7;

//const std::string systemFunctionFile = "/Users/qzq/code/compiler/splc/test/system.spl";

RoutinePart *newRoutinePartAST(const std::string &filePath) {
//	yyin = fopen((filePath).c_str(), "r");
//	yyparse();
//	return astRoot->routine->routineHead->routinePart;
	return nullptr;
}

void installSystemFunctions(Program *root) {
//	auto routinePart = root->routine->routineHead->routinePart;
//	while (routinePart->routinePart)
//		routinePart = routinePart->routinePart;
//	std::cout << routinePart << std::endl;
//	if (routinePart)
//		routinePart->routinePart = newRoutinePartAST(systemFunctionFile);
//	else
//		root->routine->routineHead->routinePart = newRoutinePartAST(systemFunctionFile);
}
