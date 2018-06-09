#include <iostream>
#include <stdio.h>
#include <fstream>
#include "AST.h"
#include "codegen.h"
#include "parser.hpp"

extern FILE *yyin;
extern Program *astRoot;

void installSystemFunctions(Program *root);

extern int yyparse();

std::ofstream astOut;
int Node::idCount = 0;

void printAST(Node *node) {
    //	std::cout << "this is " << node << std::endl;
    auto children = node->getChildren();
    std::string info = node->getInfo();
    astOut << node->id << "[label = \"" << node->getName() << (info.empty() ? "" : " : ") << info << "\"]" << std::endl;
    for (auto child : children) {
        if (child)
            astOut << node->id << " -- " << child->id << std::endl;
    }
}

void doNothing(Node *node) {}

int main(int argc, char **argv) {
	std::string sourceFile = "/Users/qzq/code/compiler/splc/test/test7.spl";
//    std::string sourceFile = "/Users/xwy/Downloads/test_examples/test1.spl";
    yyin = fopen(sourceFile.c_str(), "r");
    yyparse();

    auto root = astRoot;
    //	install system functions
//	installSystemFunctions(root);
    //visualize
    astOut.open("ast.dot", std::ios::out | std::ios::trunc);
    astOut << "graph g{" << std::endl;
    std::cout << "begin draw ast" << std::endl;
    root->traverse(printAST, doNothing);
    std::cout << "finish draw ast" << std::endl;
    astOut << "}" << std::endl;
    astOut.close();

    CodeGenContext context;
    context.generateCode(root, "output.ll");
    return 0;
}