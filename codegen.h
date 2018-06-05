//
// Created by 徐维亚 on 05/06/2018.
//

#ifndef SPLC_CODEGEN_H
#define SPLC_CODEGEN_H

#include <iostream>
#include <map>
#include <stack>
#include <utility>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "AST.h"

static llvm::LLVMContext context;

class CodeGenBlock {
public:
    llvm::BasicBlock *block;
    std::map<std::string , llvm::Value*> locals;

    explicit CodeGenBlock(llvm::BasicBlock * block) : block(block) {}
};

class CodeGenContext {
    std::stack<CodeGenBlock *> blocks;
public:
    llvm::Module * module;
    CodeGenContext(){ module = new llvm::Module("main", context);}
    void pushBlock(llvm::BasicBlock *block) { blocks.push(new CodeGenBlock(block)); }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
    void generateCode(Node& root);
};
#endif //SPLC_CODEGEN_H
