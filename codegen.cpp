//
// Created by 徐维亚 on 05/06/2018.
//

#include "codegen.h"
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/FileSystem.h>


void CodeGenContext::generateCode(Node *root) {
    std::cout << "Generating code...\n";

    /* Create the top level interpreter function to call as entry */
    std::vector<llvm::Type *> argTypes;
    llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(MyContext), makeArrayRef(argTypes),
                                                        false);
    // change GlobalValue::InternalLinkage into ExternalLinkage
    llvm::Function *mainFunction = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, "main", module);
    llvm::BasicBlock *bblock = llvm::BasicBlock::Create(MyContext, "entry", mainFunction, nullptr);


    // create print
    std::vector<llvm::Type *> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(MyContext));
    auto printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(MyContext), printf_arg_types, true);
    print = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"),
                                                    module);
    print->setCallingConv(llvm::CallingConv::C);

    /* Push a new variable/block context */
    pushBlock(bblock);
    blocks.top()->function = mainFunction;
//    currentFunction = mainFunction;
//    for (auto label:labels){
//        labelBlock[label]=BasicBlock::Create(getGlobalContext(), "label", mainFunction, 0);
//    }
//    root.CodeGen(*this); /* emit bytecode for the toplevel block */
    root->codeGen(*this);

    llvm::ReturnInst::Create(MyContext, currentBlock());
    popBlock();
//    llvm::Function * current = blocks.top()->function;
//    llvm::BasicBlock *bblock1 =llvm:: BasicBlock::Create(MyContext, "after_ret", current, nullptr);
//    pushBlock(bblock1);
//    blocks.top()->function = current;
    while (!blocks.empty()) popBlock();
    // popBlock();

    /* Print the bytecode in a human-readable format
       to see if our program compiled properly
     */
    std::cout << "Code is generated.\n";
//    llvm::PassManager pm;

//    pm.addPass(llvm::PassManager::createPrintModulePass(llvm::outs()));
//    pm.run(*module);

    // write IR to stderr
    std::cout << "code is gen~~~\n";
//    module->dump();
    llvm::outs() << *module;
    std::cout << "code is gen~!~\n";
    std::error_code ErrInfo;
    llvm::raw_ostream *out = new llvm::raw_fd_ostream("a.bc", ErrInfo, llvm::sys::fs::F_None);
    llvm::WriteBitcodeToFile(module, *out);
    out->flush();
    delete out;
}