//
// Created by QZQ on 2018/6/4.
//

#include "AST.h"
#include <vector>
#include <string>
#include "codegen.h"

using namespace llvm;

llvm::Value *Program::codeGen(CodeGenContext &context) {
//    std::vector<Type*> argTypes;
//    FunctionType *ftype = FunctionType::get(Type::getVoidTy(MyContext), makeArrayRef(argTypes), false);
//    Function * mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", context.module);
//    BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", mainFunction, 0);
    /* Push a new variable/block context */
//    context.pushBlock(bblock);
//    context.blocks.top()->function = mainFunction;
    if (routine)routine->codeGen(context); // Null ???
//    ReturnInst::Create(MyContext, bblock);
//    context.popBlock();
    return nullptr;
}

llvm::Value *Routine::codeGen(CodeGenContext &context) {
    routineHead->codeGen(context);
    routineBody->codeGen(context);
    return nullptr;
}

llvm::Value *RoutineHead::codeGen(CodeGenContext &context) {
    if (labelPart) labelPart->codeGen(context);
    if (constPart) constPart->codeGen(context);
    if (typePart) typePart->codeGen(context);
    if (varPart) varPart->codeGen(context);
    routinePart->codeGen(context);
    return nullptr;
}

llvm::Value *ConstPart::codeGen(CodeGenContext &context) {
    if (constExprList) return constExprList->codeGen(context);
    return nullptr;
}

//static Type *typeOf(const int type) {
//    switch (type) {
//        case ConstValue::T_INTEGER:
//            return Type::getInt32Ty(MyContext);
//        case ConstValue::T_CHAR:
//            return Type::getInt8Ty(MyContext); // No char?
//        case ConstValue::T_REAL:
//            return Type::getDoubleTy(MyContext);
//        case ConstValue::T_STRING:
//            return nullptr; // ?????????/ Array?
//        default:
//            return nullptr;
//    }
//}

llvm::Value *ConstExprList::codeGen(CodeGenContext &context) {
    if (preList) preList->codeGen(context); //顺序？从前往后？从后往前？
    auto var = value->codeGen(context);
    AllocaInst *alloc = new AllocaInst(var->getType(), 0, name, context.currentBlock()); // 那个1是干什么的呢
    context.local()[name] = alloc;
    return new StoreInst(var, context.local()[name], false, context.currentBlock());
}

llvm::Value *ConstValue::codeGen(CodeGenContext &context) {
    switch (type) {
        case ConstValue::T_INTEGER:
            return ConstantInt::get(Type::getInt32Ty(MyContext), std::stoi(value), true); // unsigned???
        case ConstValue::T_CHAR:
            return ConstantInt::get(Type::getInt8Ty(MyContext), value.at(0), true);
        case ConstValue::T_REAL:
            return llvm::ConstantFP::get(MyContext, llvm::APFloat(std::stod(value)));
        case ConstValue::T_SYS_CON:
            if (value == "maxint")
                return ConstantInt::get(Type::getInt32Ty(MyContext), 2147483647, true);
            else if(value == "false")
                return ConstantInt::get(Type::getInt1Ty(MyContext), 1, true);
            else if(value == "true")
                return ConstantInt::get(Type::getInt1Ty(MyContext), 0, true);
            return nullptr; // ?????????/ Array?
        default:
            return nullptr;
    }
}

llvm::Value *TypePart::codeGen(CodeGenContext &context) {
    if (typeDeclList) return typeDeclList->codeGen(context);
    return nullptr;
}

llvm::Value *TypeDeclList::codeGen(CodeGenContext &context) {
    if (preList)preList->codeGen(context);
    return typeDefinition->codeGen(context);
}

llvm::Value *TypeDefinition::codeGen(CodeGenContext &context) {
    context.type()[name] = typeDecl; //需要查找已经有了么
    return nullptr;
}

llvm::Value *VarPart::codeGen(CodeGenContext &context) {
    if (varDeclList) {
        return varDeclList->codeGen(context);
    }
    return nullptr;
}

llvm::Value *VarDeclList::codeGen(CodeGenContext &context) {
    if (preList) preList->codeGen(context);//顺序？从前往后？从后往前？
    return varDecl->codeGen(context);
}

llvm::Value *VarDecl::codeGen(CodeGenContext &context) {
    NameList *n = nameList;
    while (n) {
        // how to get type
        llvm::Type *t = typeDecl->getType(context);
        if (!t) {
            std::cout << "Error: undefined type." << std::endl;
            exit(1);
//            return nullptr;
        }
        AllocaInst *alloc = new AllocaInst(t, 0, n->name, context.currentBlock()); // 那个1是干什么的呢
        context.local()[n->name] = alloc;
        n = n->nameList;
    }
    return nullptr;
}

llvm::Type *TypeDecl::getType(CodeGenContext &context) {
    if (type == T_SIMPLE_TYPE_DECLARE) return simpleTypeDecl->getType(context);
    else if (type == T_ARRAY_TYPE_DECLARE) return arrayTypeDecl->getType(context);
    else if (type == T_RECORD_TYPE_DECLARE) return recordTypeDecl->getType(context);
    return nullptr;
};

llvm::Type *SimpleTypeDecl::getType(CodeGenContext &context) {
    switch (type) {
        case T_SYS_TYPE:
            if (sysType == "boolean") return llvm::Type::getInt1Ty(MyContext);
            else if (sysType == "char") return llvm::Type::getInt1Ty(MyContext);
            else if (sysType == "integer") return llvm::Type::getInt32Ty(MyContext);
            else return llvm::Type::getDoubleTy(MyContext);
        case T_TYPE_NAME: {
            if (name == "boolean") return llvm::Type::getInt1Ty(MyContext);
            else if (name == "char") return llvm::Type::getInt1Ty(MyContext);
            else if (name == "integer") return llvm::Type::getInt32Ty(MyContext);
//            else return llvm::Type::getDoubleTy(MyContext);
            CodeGenBlock *p = context.blocks.top();
            while (p) {
                if (p->types.find(name) == p->types.end()) {
                    p = p->preBlock;
                    continue;
                }
                return p->types.at(name)->getType(context);
            }
            return nullptr;
        }
        default:
            break;
    }
    return nullptr;
};

llvm::Type *ArrayTypeDecl::getType(CodeGenContext &context) {
    return nullptr;
}

llvm::Type *RecordTypeDecl::getType(CodeGenContext &context) {
    return nullptr;
}

llvm::Value *RoutinePart::codeGen(CodeGenContext &context) {
    if (type == T_ROUTINE_FUNC || type == T_ROUTINE_PROC) return routinePart->codeGen(context);
    if (type == T_EMPTY)
        return nullptr;
    if (type == T_PROC) return procedureDecl->codeGen(context);
    if (type == T_FUNC) return functionDecl->codeGen(context);
    return nullptr;
}

llvm::Value *FunctionDecl::codeGen(CodeGenContext &context) {
    CodeGenBlock *parent = context.blocks.top();
    std::vector<Type *> argTypes; // size = 1 ?? should be 2
    ParaDeclList *p = functionHead->parameters->paraDeclList;
    while (p) {

        NameList *n;
        if (p->paraTypeList->type == ParaTypeList::T_VAL) {
            n = p->paraTypeList->valParaList->nameList;
        } else {
            n = p->paraTypeList->varParaList->nameList;
        }
        while (n) {
            argTypes.push_back(p->paraTypeList->typeDecl->getType(context));
            n = n->nameList;
        }
        p = p->paraDeclList;
    } // 反正倒序定义的到时候倒序访问应该不成问题假装
    FunctionType *ftype = FunctionType::get(functionHead->returnType->getType(context), makeArrayRef(argTypes), false);
    Function *function = Function::Create(ftype, llvm::GlobalValue::InternalLinkage, functionHead->name,
                                          context.module);
    BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", function, nullptr);
    context.pushBlock(bblock);
    context.blocks.top()->function = function;
    p = functionHead->parameters->paraDeclList;
    llvm::Value *arg_value;
    auto args_values = function->arg_begin();
    std::vector<llvm::Value *> var;
    std::vector<int> place;
    int i = 0;
    while (p) {
        NameList *n;
//        p->codeGen(context);
        if (p->paraTypeList->type == ParaTypeList::T_VAL) {
            n = p->paraTypeList->valParaList->nameList;
        } else {
            n = p->paraTypeList->varParaList->nameList;
        }
        while (n) {
            if (p->paraTypeList->type == ParaTypeList::T_VAR) {
                auto initial = llvm::Constant::getNullValue(p->paraTypeList->typeDecl->getType(context));
                auto go = new llvm::GlobalVariable(*context.module, p->paraTypeList->typeDecl->getType(context), false,
                                                   llvm::GlobalValue::ExternalLinkage, initial);
                context.local()[n->name] = go;
                new llvm::StoreInst(args_values, go, false, context.currentBlock());
                var.push_back(go);
                place.push_back(i);
            } else {
                AllocaInst *alloc = new AllocaInst(p->paraTypeList->typeDecl->getType(context), 0, n->name,
                                                   context.currentBlock()); // 那个1是干什么的呢
                context.local()[n->name] = alloc;
//            arg_value = args_values++;
//            arg_value->setName(n->name);
                new llvm::StoreInst(args_values, alloc, false, context.currentBlock());
            }
            i++;
            args_values++;
            n = n->nameList;
        }
        p = p->paraDeclList;
    }
    if (context.funcVars.find(functionHead->name) != context.funcVars.end()) {
        std::cout << "Error, redeclare function: " << functionHead->name;
        exit(0);
    }
    context.funcVars[functionHead->name].storePlace = var;
    context.funcVars[functionHead->name].position = place;
    AllocaInst *alloc = new AllocaInst(functionHead->returnType->getType(context), 0, functionHead->name,
                                       context.currentBlock()); // 那个1是干什么的呢
    context.local()[functionHead->name] = alloc;

    subRoutine->codeGen(context);

// Load return value
    auto retVal = new LoadInst(alloc, "", false, context.currentBlock());
    llvm::ReturnInst::Create(MyContext, retVal, bblock
    );
    context.popBlock();

    while (context.blocks.top() != parent)context.popBlock();
//    Function * current = context.blocks.top()->function;
//    BasicBlock *bblock1 = BasicBlock::Create(MyContext, "after_ret", current, nullptr);
//    context.pushBlock(bblock1);
//    context.blocks.top()->function = current;
    return function;
}

llvm::Value *ProcedureDecl::codeGen(CodeGenContext &context) {
    std::vector<Type *> argTypes;
    ParaDeclList *p = procedureHead->parameters->paraDeclList;
    while (p) {
        NameList *n = p->paraTypeList->valParaList->nameList;
        while (n) {
            argTypes.push_back(p->paraTypeList->typeDecl->getType(context));
            n = n->nameList;
        }
        p = p->paraDeclList;
    }
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(MyContext), makeArrayRef(argTypes), false);
    Function *function = Function::Create(ftype, llvm::GlobalValue::InternalLinkage, procedureHead->name,
                                          context.module);
    BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", function, nullptr);
    context.pushBlock(bblock);
    context.blocks.top()->function = function;
    llvm::Value *arg_value;
    auto args_values = function->arg_begin();
    p = procedureHead->parameters->paraDeclList;
    while (p) {
        if (p->paraTypeList->type == ParaTypeList::T_VAL) {
            NameList *n = p->paraTypeList->valParaList->nameList;
            while (n) {
                AllocaInst *alloc = new AllocaInst(p->paraTypeList->typeDecl->getType(context), 0, n->name,
                                                   context.currentBlock()); // 那个1是干什么的呢
                context.local()[n->name] = alloc;
                arg_value = args_values++;
                arg_value->setName(n->name);
                auto inst = new llvm::StoreInst(arg_value, alloc, false, context.currentBlock());
                n = n->nameList;
            }
        } else {
            NameList *n = p->paraTypeList->varParaList->nameList;
            while (n) {
//            AllocaInst *alloc = new AllocaInst(paraTypeList->typeDecl->getType(context), 0, n->name,
//                                               context.currentBlock()); // 那个1是干什么的呢
                std::string s = args_values->getName();
                std::cout << s << std::endl;
                auto pp = context.blocks.top();
                while (pp) {
                    if (pp->locals.find(s) == pp->locals.end()) {
                        pp = pp->preBlock;
                        continue;
                    }
                    if (pp->locals[s] == nullptr) {
                        std::cout << "Uninitialize variable: " << n->name << std::endl;
                    }
                    context.local()[n->name] = pp->locals[s];
                    break;
                }
//            context.blocks.top()->alias[n->name]
//            if(n->name == "j"){
//                std::cout << "ParaDecl.j" << std::endl;
//            }
                n = n->nameList;
            }
        }
        p = p->paraDeclList;
    }
    subRoutine->codeGen(context);
    context.popBlock();
    return function;
}

llvm::Value *SubRoutine::codeGen(CodeGenContext &context) {
    routineHead->codeGen(context);
    return routineBody->codeGen(context);
};

llvm::Value *ParaDeclList::codeGen(CodeGenContext &context) {
    // var是引用
    if (paraTypeList->type == ParaTypeList::T_VAL) {
        NameList *n = paraTypeList->valParaList->nameList;
        while (n) {
            AllocaInst *alloc = new AllocaInst(paraTypeList->typeDecl->getType(context), 0, n->name,
                                               context.currentBlock()); // 那个1是干什么的呢
            context.local()[n->name] = alloc;
            n = n->nameList;
        }
    } else {
        NameList *n = paraTypeList->varParaList->nameList;
        while (n) {
//            AllocaInst *alloc = new AllocaInst(paraTypeList->typeDecl->getType(context), 0, n->name,
//                                               context.currentBlock()); // 那个1是干什么的呢
            context.local()[n->name] = nullptr;
//            context.blocks.top()->alias[n->name]
//            if(n->name == "j"){
//                std::cout << "ParaDecl.j" << std::endl;
//            }
            n = n->nameList;
        }
    }
    return nullptr;
}

llvm::Value *RoutineBody::codeGen(CodeGenContext &context) {
    return compoundStmt->codeGen(context);
}

llvm::Value *CompoundStmt::codeGen(CodeGenContext &context) {
    return stmtList->codeGen(context);
}

llvm::Value *StmtList::codeGen(CodeGenContext &context) {
    if (preList) preList->codeGen(context);
    if (stmt) stmt->codeGen(context);
    return nullptr;
}

llvm::Value *Stmt::codeGen(CodeGenContext &context) {
    nonLabelStmt->codeGen(context);
    return nullptr;
}

llvm::Value *NonLabelStmt::codeGen(CodeGenContext &context) {
    switch (type) {
        case T_ASSIGN:
            return assignStmt->codeGen(context);
        case T_PROC:
            return procStmt->codeGen(context);
        case T_IF:
            return ifStmt->codeGen(context);
        case T_REPEAT:
            return repeatStmt->codeGen(context);
        case T_CASE:
            return caseStmt->codeGen(context);
        case T_COMPOUND:
            return compoundStmt->codeGen(context);
        case T_GOTO:
            return gotoStmt->codeGen(context);
        case T_FOR:
            return forStmt->codeGen(context);
        case T_WHILE:
            return whileStmt->codeGen(context);
        default:
            return nullptr;
    }
}

void getPrintArgs(std::vector<llvm::Value *> &printf_args, std::string &printf_format, ExpressionList *p,
                  CodeGenContext &context) {
    if (p) {
        if (p->preList) getPrintArgs(printf_args, printf_format, p->preList, context);
        auto arg_val = p->expression->codeGen(context);
        if (arg_val->getType() == llvm::Type::getInt32Ty(MyContext)) {
            printf_format += "%d ";
            std::cout << "SysFuncCall write variable previous name" << arg_val->getName().str() << std::endl;
            printf_args.push_back(arg_val);
        } else if (arg_val->getType()->isDoubleTy() /*== llvm::Type::getDoubleTy(llvm::getGlobalContext())*/) {
            printf_format += "%lf";
            printf_args.push_back(arg_val);
        } else if (arg_val->getType() == llvm::Type::getInt8PtrTy(MyContext)) {
            assert("print string" == "not implemented");
        }
    }
}

llvm::Value *funcGen(CodeGenContext &context, std::string &procId, ArgsList *argsList) {
    Function *function = context.module->getFunction(procId);
    if (function == nullptr) {
        std::cout << "Function/procedure called but not declared: " << procId << std::endl;
        exit(0);
    }
    std::vector<Value *> args;
    auto p = argsList;
    while (p) {
        args.push_back(p->expression->codeGen(context));
        p = p->preList;
    } //确定了前面是这个顺序定义的
    auto call = llvm::CallInst::Create(function, llvm::makeArrayRef(args), "", context.currentBlock());
    if (!context.funcVars[procId].position.empty()) {
        // 处理引用
        p = argsList;
//        int j = 0, k = 0;
        int k = 0;
        auto i = context.funcVars[procId].storePlace.begin();
        auto j = context.funcVars[procId].position.begin();
        while (p) {
            if (i == context.funcVars[procId].storePlace.end()) break;
            if (k == *j) {
                auto tmp = new llvm::LoadInst(*i, "", false, context.currentBlock()); // ??
                i++;
                j++;
                if (!p->expression || p->expression->type != Expression::T_EXPR ||
                    p->expression->expr->type != Expr::T_TERM ||
                    p->expression->expr->term->type != Term::T_FACTOR ||
                    p->expression->expr->term->factor->type != Factor::T_NAME) {
                    std::cout << "Reference must pass a variable." << std::endl;
                    exit(0);
                }
                auto name = p->expression->expr->term->factor->name;
                auto b = context.blocks.top();
                while (b) {
                    if (b->locals.find(name) == b->locals.end()) {
                        b = b->preBlock;
                        continue;
                    }
                    new llvm::StoreInst(tmp, b->locals[name], false, context.currentBlock());
                    break;
                }
            }
            p = p->preList;
            k++;
        }

    }
    return call;
}

llvm::Value *ProcStmt::codeGen(CodeGenContext &context) {
    if (type == T_SIMPLE || type == T_SIMPLE_ARGS) {
        return funcGen(context, procId, argsList);
    } else if (type == T_SYS_PROC_EXPR) {
        if (sysProc == "write" || sysProc == "writeln") {
            std::string printf_format;
            std::vector<llvm::Value *> printf_args;

            ExpressionList *p = expressionList;
            getPrintArgs(printf_args, printf_format, p, context);
//            if (sysProc == "writeln")
            printf_format += "\n";
//            else printf_format = printf_format.substr(0, printf_format.size()-1);
//            std::cout << printf_format << std::endl;

            auto printf_format_const = llvm::ConstantDataArray::getString(MyContext, printf_format, true);
            auto format_string_var = new llvm::GlobalVariable(*context.module,
                                                              llvm::ArrayType::get(llvm::IntegerType::get(MyContext, 8),
                                                                                   printf_format.size() + 1), true,
                                                              llvm::GlobalValue::PrivateLinkage, printf_format_const,
                                                              ".str");
            auto zero = llvm::Constant::getNullValue(llvm::IntegerType::getInt32Ty(MyContext));

            std::vector<llvm::Constant *> indices;
            indices.push_back(zero);
            indices.push_back(zero);
            auto var_ref = llvm::ConstantExpr::getGetElementPtr(
                    llvm::ArrayType::get(llvm::IntegerType::get(MyContext, 8), printf_format.size() + 1),
                    format_string_var,
                    indices);

            printf_args.insert(printf_args.begin(), var_ref);
            auto call = llvm::CallInst::Create(context.print, llvm::makeArrayRef(printf_args), "",
                                               context.currentBlock());
            return call;
        }
    }
    return nullptr;
}

llvm::Value *AssignStmt::codeGen(CodeGenContext &context) {
    CodeGenBlock *b = context.blocks.top();
    while (b) {
        if (b->locals.find(id) == b->locals.end()) {
            b = b->preBlock;
            continue;
        }
        return new llvm::StoreInst(rhs->codeGen(context), b->locals[id], false, context.currentBlock());
    }
    return nullptr;
}

llvm::Value *IfStmt::codeGen(CodeGenContext &context) {
    Value *condition = expression->codeGen(context);
    BasicBlock *btrue = BasicBlock::Create(MyContext, "thenStmt", context.blocks.top()->function);
    BasicBlock *bfalse = BasicBlock::Create(MyContext, "elseStmt", context.blocks.top()->function);
    BasicBlock *bmerge = BasicBlock::Create(MyContext, "mergeStmt", context.blocks.top()->function);
    llvm::Instruction *ret = llvm::BranchInst::Create(btrue, bfalse, condition, context.currentBlock());
    context.pushBlock(btrue);

    stmt->codeGen(context);
    llvm::BranchInst::Create(bmerge, context.currentBlock());
    context.popBlock();
    context.pushBlock(bfalse);
    if (elseClause)
        elseClause->codeGen(context);
    llvm::BranchInst::Create(bmerge, context.currentBlock());
    context.popBlock();
    context.pushBlock(bmerge);
//    context.popBlock();//??
    return ret;
}

llvm::Value *ElseClause::codeGen(CodeGenContext &context) {
    if (stmt) return stmt->codeGen(context);
    else return nullptr;
}

llvm::Value *WhileStmt::codeGen(CodeGenContext &context) {
    Function *currentFuction = context.blocks.top()->function;
    BasicBlock *sloop = BasicBlock::Create(MyContext, "startloop", currentFuction);
    BasicBlock *bloop = BasicBlock::Create(MyContext, "loopStmt", currentFuction);
    BasicBlock *bexit = BasicBlock::Create(MyContext, "eixtStmt", currentFuction);

    llvm::BranchInst::Create(sloop, context.currentBlock());
    context.pushBlock(sloop);
    context.blocks.top()->function = currentFuction;
    Value *test = whileCondition->codeGen(context);
    llvm::Instruction *ret = llvm::BranchInst::Create(bloop, bexit, test, context.currentBlock());
    context.popBlock();
    context.pushBlock(bloop);
    context.blocks.top()->function = currentFuction;
    stmt->codeGen(context);
    llvm::BranchInst::Create(sloop, context.currentBlock());
    context.popBlock();
    context.pushBlock(bexit);
    context.blocks.top()->function = currentFuction;
//    context.popBlock(); // ??
    return ret;
}

llvm::Value *RepeatStmt::codeGen(CodeGenContext &context) {
    Function *currentFuction = context.blocks.top()->function;
    BasicBlock *bloop = BasicBlock::Create(MyContext, "loopStmt", currentFuction);
    BasicBlock *bexit = BasicBlock::Create(MyContext, "eixtStmt", currentFuction);
    llvm::BranchInst::Create(bloop, context.currentBlock());

    context.pushBlock(bloop);
    context.blocks.top()->function = currentFuction;

    stmtList->codeGen(context);
    Value *test = untilCondition->codeGen(context);
    llvm::Instruction *ret = llvm::BranchInst::Create(bexit, bloop, test, context.currentBlock());
    context.popBlock();

    context.pushBlock(bexit);
    context.blocks.top()->function = currentFuction;

//    context.popBlock(); // ??
    return ret;
}

llvm::Value *Expression::codeGen(CodeGenContext &context) {
    if (type == T_EXPR) {
        return expr->codeGen(context);
    } else {
        Value *op1_val = expression->codeGen(context);
        Value *op2_val = expr->codeGen(context);
        switch (type) {
            case T_EQ:
                return llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::CmpInst::ICMP_EQ,
                                             op1_val, op2_val, "", context.currentBlock());
            case T_NE:
                return llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::CmpInst::ICMP_NE,
                                             op1_val, op2_val, "", context.currentBlock());
            case T_LT:
                return llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::CmpInst::ICMP_SLT,
                                             op1_val, op2_val, "", context.currentBlock());
            case T_GT:
                return llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::CmpInst::ICMP_SGT,
                                             op1_val, op2_val, "", context.currentBlock());
            case T_LE:
                return llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::CmpInst::ICMP_SLE,
                                             op1_val, op2_val, "", context.currentBlock());
            case T_GE:
                return llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::CmpInst::ICMP_SGE,
                                             op1_val, op2_val, "", context.currentBlock());
            default:
                return nullptr;
        }
    }
}

llvm::Value *Expr::codeGen(CodeGenContext &context) {
    if (type == T_TERM) return term->codeGen(context);
    Value *op1_val = expr->codeGen(context);
    Value *op2_val = term->codeGen(context);
    switch (type) {
        case T_PLUS:
            return llvm::BinaryOperator::Create(llvm::Instruction::Add,
                                                op1_val, op2_val, "", context.currentBlock());
        case T_MINUS:
            return llvm::BinaryOperator::Create(llvm::Instruction::Sub,
                                                op1_val, op2_val, "", context.currentBlock());
        case T_OR:
            return llvm::BinaryOperator::Create(llvm::Instruction::Or,
                                                op1_val, op2_val, "", context.currentBlock());
        default:
            return nullptr;
    }
}

llvm::Value *Term::codeGen(CodeGenContext &context) {
    if (type == T_FACTOR) return factor->codeGen(context);
    Value *op1_val = term->codeGen(context);
    Value *op2_val = factor->codeGen(context);
    switch (type) {
        case T_MUL:
            return llvm::BinaryOperator::Create(llvm::Instruction::Mul,
                                                op1_val, op2_val, "", context.currentBlock());
        case T_DIV:
            return llvm::BinaryOperator::Create(llvm::Instruction::SDiv,
                                                op1_val, op2_val, "", context.currentBlock());
        case T_AND:
            return llvm::BinaryOperator::Create(llvm::Instruction::And,
                                                op1_val, op2_val, "", context.currentBlock());
        case T_MOD:
            return llvm::BinaryOperator::Create(llvm::Instruction::SRem,
                                                op1_val, op2_val, "", context.currentBlock());
        default:
            return nullptr;
    }
}

llvm::Value *Factor::codeGen(CodeGenContext &context) {
    switch (type) {
        case T_NAME: {
            auto p = context.blocks.top();
            while (p) {
                if (p->locals.find(name) == p->locals.end()) {
                    p = p->preBlock;
                    continue;
                }
                if (p->locals[name] == nullptr) {
                    std::cout << "Uninitialize variable: " << name << std::endl;
                }
                return new llvm::LoadInst(p->locals[name], "", false, context.currentBlock()); // ??
            }
            std::cout << "Undefined variable: " << name << std::endl;
            exit(1);
        }
        case T_CONST:
            return constValue->codeGen(context);
        case T_EXPR:
            return expression->codeGen(context);
        case T_NOT_FACTOR:
            return nullptr; // not finished
        case T_NAME_ARGS:
            return funcGen(context, name, argsList);
        default:
            return nullptr;
    }
}

llvm::Value *ForStmt::codeGen(CodeGenContext &context) {
    Function *currentFuction = context.blocks.top()->function;
    BasicBlock *sloop = BasicBlock::Create(MyContext, "startloop", currentFuction);
    BasicBlock *bloop = BasicBlock::Create(MyContext, "loopStmt", currentFuction);
    BasicBlock *bexit = BasicBlock::Create(MyContext, "eixtStmt", currentFuction);
//  initial for
    AssignStmt *initial = new AssignStmt(loopId, firstBound);
    initial->codeGen(context);
    llvm::BranchInst::Create(sloop, context.currentBlock());
//  for test
    context.pushBlock(sloop);
    context.blocks.top()->function = currentFuction;

    auto *f = new Factor(Factor::T_NAME, loopId);
    Value *test = llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::CmpInst::ICMP_EQ,
                                        f->codeGen(context), secondBound->codeGen(context), "", context.currentBlock());
    llvm::Instruction *ret = llvm::BranchInst::Create(bexit, bloop, test, context.currentBlock());
    context.popBlock();

    context.pushBlock(bloop);
    context.blocks.top()->function = currentFuction;
    stmt->codeGen(context);
//update
    Factor *f1;
    auto int1 = new ConstValue("1", ConstValue::T_INTEGER);
    Value *update;
    if (direction->type == Direction::T_TO) {
        f1 = new Factor(Factor::T_NAME, loopId);
        update = llvm::BinaryOperator::Create(llvm::Instruction::Add,
                                              f1->codeGen(context), int1->codeGen(context), "", context.currentBlock());
    } else {
        f1 = new Factor(Factor::T_NAME, loopId);
        update = llvm::BinaryOperator::Create(llvm::Instruction::Sub,
                                              f1->codeGen(context), int1->codeGen(context), "", context.currentBlock());
    }
    CodeGenBlock *b = context.blocks.top();
    while (b) {
        if (b->locals.find(loopId) == b->locals.end()) {
            b = b->preBlock;
            continue;
        }
        new llvm::StoreInst(update, b->locals[loopId], false, context.currentBlock());
        break;
    }
    llvm::BranchInst::Create(sloop, context.currentBlock());
    context.popBlock();

    context.pushBlock(bexit);
    context.blocks.top()->function = currentFuction;

//    context.popBlock(); // ??
    stmt->codeGen(context); // why ? -- 因为包括右边的
    delete initial;
    delete f;
    delete int1;
//    delete update;
    delete f1;
    return ret;
}
