//
// Created by 徐维亚 on 05/06/2018.
//

#ifndef SPLC_CODEGEN_H
#define SPLC_CODEGEN_H

#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <utility>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/IRBuilder.h>
#include "AST.h"
#include "ConstTable.h"

static llvm::LLVMContext MyContext;

class Node;
class FunctionHead;

class FuncVars {
public:
    std::vector<llvm::Value *> storePlace;
    std::vector<int> position;
};

class CodeGenBlock {
public:
    llvm::BasicBlock *block;
    CodeGenBlock *preBlock;
    llvm::Function *function;
    std::map<std::string, llvm::Value *> locals;
    std::map<std::string, TypeDecl *> types;
    std::string outputFilename;

    explicit CodeGenBlock(llvm::BasicBlock *block, CodeGenBlock *preBlock) : block(block), preBlock(preBlock) {}
};

class CodeGenContext {
public:
    std::stack<CodeGenBlock *> blocks;
    llvm::Module *module;
    std::map<std::string, FuncVars> funcVars;
    ConstTable constTtable;

    CodeGenContext() : module(new llvm::Module("main", MyContext)), print(nullptr) {}
    ~CodeGenContext() {
        delete module;
    }

    void pushBlock(llvm::BasicBlock *block) {
        CodeGenBlock *top = nullptr;
        if(!blocks.empty()) {
            top = blocks.top();
        }
        blocks.push(new CodeGenBlock(block, top));
    }

    void popBlock() {
        CodeGenBlock *top = blocks.top();
        blocks.pop();
        delete top;
    }

    std::map<std::string, llvm::Value *> & local() { return blocks.top()->locals; };

    std::map<std::string, TypeDecl *> type() { return blocks.top()->types; };

    llvm::BasicBlock *currentBlock() { return blocks.top()->block; }

    void generateCode(Node *root, const std::string &outputFilename);

    llvm::Function *print;
};

#endif //SPLC_CODEGEN_H
