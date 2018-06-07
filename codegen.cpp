//
// Created by 徐维亚 on 05/06/2018.
//

#include "codegen.h"

void CodeGenContext::generateCode(Node * root) {
    std::vector<llvm::Type *> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(MyContext));
    auto printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(MyContext), printf_arg_types, true);
    auto func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), module);
    func->setCallingConv(llvm::CallingConv::C);
    root->codeGen(*this);
}