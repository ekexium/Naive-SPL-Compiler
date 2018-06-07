//
// Created by QZQ on 2018/6/4.
//

#include "AST.h"
#include <vector>
#include <string>
#include "codegen.h"

using namespace llvm;

llvm::Value * Program::codeGen(CodeGenContext &context) {
    std::vector<Type*> argTypes;
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(MyContext), makeArrayRef(argTypes), false);
    Function * mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", context.module);
    BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", mainFunction, 0);
    /* Push a new variable/block context */
    context.pushBlock(bblock);
    context.blocks.top()->function = mainFunction;
    if(routine)routine->codeGen(context); // Null ???
    ReturnInst::Create(MyContext, bblock);
    context.popBlock();
    return nullptr;
}

llvm::Value * Routine::codeGen(CodeGenContext &context) {
    routineHead->codeGen(context);
    routineBody->codeGen(context);
    return nullptr;
}

llvm::Value * RoutineHead::codeGen(CodeGenContext &context) {
    if(labelPart) labelPart->codeGen(context);
    if(constPart) constPart->codeGen(context);
    if(typePart) typePart->codeGen(context);
    if(varPart) varPart->codeGen(context);
    routinePart->codeGen(context);
    return nullptr;
}

llvm::Value * ConstPart::codeGen(CodeGenContext &context) {
    if(constExprList) return constExprList->codeGen(context);
    return nullptr;
}

static Type * typeOf(const int type) {
    switch (type) {
        case ConstValue::T_INTEGER:
            return Type::getInt64Ty(MyContext);
        case ConstValue::T_CHAR:
            return Type::getInt8Ty(MyContext); // No char?
        case ConstValue::T_REAL:
            return Type::getDoubleTy(MyContext);
        case ConstValue::T_STRING:
            return nullptr; // ?????????/ Array?
        default:
            return nullptr;
    }
}

llvm::Value * ConstExprList::codeGen(CodeGenContext &context) {
    if(preList) preList->codeGen(context); //顺序？从前往后？从后往前？
    AllocaInst *alloc = new AllocaInst(typeOf(value->type), 1, name, context.currentBlock()); // 那个1是干什么的呢
    context.local()[name] = alloc;
    new StoreInst(value->codeGen(context), context.local()[name], false, context.currentBlock());
}

llvm::Value * ConstValue::codeGen(CodeGenContext &context) {
    switch (type) {
        case ConstValue::T_INTEGER:
            return ConstantInt::get(Type::getInt64Ty(MyContext), std::stoi(value), true); // unsigned???
        case ConstValue::T_CHAR:
            return ConstantInt::get(Type::getInt8Ty(MyContext), value[0], true);
        case ConstValue::T_REAL:
            return llvm::ConstantFP::get(MyContext, llvm::APFloat(std::stod(value)));
        case ConstValue::T_STRING:
            return nullptr; // ?????????/ Array?
        default:
            return nullptr;
    }
}

llvm::Value * TypePart::codeGen(CodeGenContext &context) {
    return typeDeclList->codeGen(context);
}

llvm::Value * TypeDeclList::codeGen(CodeGenContext &context) {
    preList->codeGen(context);
    return typeDefinition->codeGen(context);
}

llvm::Value * TypeDefinition::codeGen(CodeGenContext &context){
    context.type()[name] = typeDecl; //需要查找已经有了么
    return nullptr;
}

llvm::Value * VarPart::codeGen(CodeGenContext &context) {
    return varDeclList->codeGen(context);
}

llvm::Value * VarDeclList::codeGen(CodeGenContext &context) {
    if(preList) preList->codeGen(context);//顺序？从前往后？从后往前？
    return varDecl->codeGen(context);
}

llvm::Value * VarDecl::codeGen(CodeGenContext &context) {
    NameList * n = nameList;
    while (n) {
        // how to get type
        AllocaInst *alloc = new AllocaInst(typeDecl->getType(context), 1, n->name, context.currentBlock()); // 那个1是干什么的呢
        context.local()[n->name] = alloc;
        n = n->nameList;
    }
}

llvm::Type * TypeDecl::getType(CodeGenContext & context) {
    if(type == T_RECORD_TYPE_DECLARE) return simpleTypeDecl->getType(context);
    else if(type == T_ARRAY_TYPE_DECLARE) return arrayTypeDecl->getType(context);
    else if(type == T_RECORD_TYPE_DECLARE) return recordTypeDecl->getType(context);
    return nullptr;
};

llvm::Type * SimpleTypeDecl::getType(CodeGenContext & context) {
    switch (type) {
        case T_SYS_TYPE:
            if(sysType == "boolean") return llvm::Type::getInt1Ty(MyContext);
            else if(sysType == "char") return llvm::Type::getInt1Ty(MyContext);
            else if(sysType == "integer") return llvm::Type::getInt64Ty(MyContext);
            else return llvm::Type::getDoubleTy(MyContext);
        case T_TYPE_NAME: {
            CodeGenBlock *p = context.blocks.top();
            while (p) {
                if (p->types.find(name) == p->types.end()) p = p->preBlock;
                return p->types.at(name)->getType(context);
            }
            return nullptr;
        }
        default:
            break;
    }
    return nullptr;
};

llvm::Type * ArrayTypeDecl::getType(CodeGenContext & context) {
    return nullptr;
}

llvm::Type * RecordTypeDecl::getType(CodeGenContext &context) {
    return nullptr;
}

llvm::Value * RoutinePart::codeGen(CodeGenContext &context) {
    if(type == T_ROUTINE_FUNC || type == T_ROUTINE_PROC) return routinePart->codeGen(context);
    if(type == T_EMPTY)
        return nullptr;
    if(type == T_PROC) return procedureDecl->codeGen(context);
    if(type == T_FUNC) return functionDecl->codeGen(context);
    return nullptr;
}

llvm::Value * FunctionDecl::codeGen(CodeGenContext &context) {
    std::vector<Type*> argTypes;
    ParaDeclList * p = functionHead->parameters->paraDeclList;
    while (p) {
        argTypes.push_back(p->paraTypeList->typeDecl->getType(context));
        p = p->paraDeclList;
    }
    FunctionType *ftype = FunctionType::get(functionHead->returnType->getType(context), makeArrayRef(argTypes), false);
    Function * function = Function::Create(ftype, llvm::GlobalValue::InternalLinkage, functionHead->name, context.module);
    BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", function, nullptr);
    context.pushBlock(bblock);
    context.blocks.top()->function = function;
    p = functionHead->parameters->paraDeclList;
    while (p){
        p->codeGen(context);
        p = p->paraDeclList;
    }

    AllocaInst *alloc = new AllocaInst(functionHead->returnType->getType(context), 1, functionHead->name, context.currentBlock()); // 那个1是干什么的呢
    context.local()[functionHead->name] = alloc;
    subRoutine->codeGen(context);
    context.popBlock();
    return function;
}

llvm::Value * ProcedureDecl::codeGen(CodeGenContext &context) {
    std::vector<Type*> argTypes;
    ParaDeclList * p = procedureHead->parameters->paraDeclList;
    while (p) {
        argTypes.push_back(p->paraTypeList->typeDecl->getType(context));
        p = p->paraDeclList;
    }
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(MyContext), makeArrayRef(argTypes), false);
    Function * function = Function::Create(ftype, llvm::GlobalValue::InternalLinkage, procedureHead->name, context.module);
    BasicBlock *bblock = BasicBlock::Create(MyContext, "entry", function, nullptr);
    context.pushBlock(bblock);
    context.blocks.top()->function = function;
    p = procedureHead->parameters->paraDeclList;
    while (p){
        p->codeGen(context);
        p = p->paraDeclList;
    }
    subRoutine->codeGen(context);
    context.popBlock();
    return function;
}

llvm::Value * SubRoutine::codeGen(CodeGenContext &context) {
    routineHead->codeGen(context);
    return routineBody->codeGen(context);
};

llvm::Value * ParaDeclList::codeGen(CodeGenContext &context) {
    if(paraTypeList->type == ParaTypeList::T_VAL) {
        NameList * n = paraTypeList->valParaList->nameList;
        while (n) {
            AllocaInst *alloc = new AllocaInst(paraTypeList->typeDecl->getType(context), 1, n->name, context.currentBlock()); // 那个1是干什么的呢
            context.local()[n->name] = alloc;
            n = n->nameList;
        }
    } else {
        NameList * n = paraTypeList->varParaList->nameList;
        while (n) {
            AllocaInst *alloc = new AllocaInst(paraTypeList->typeDecl->getType(context), 1, n->name, context.currentBlock()); // 那个1是干什么的呢
            context.local()[n->name] = alloc;
            n = n->nameList;
        }
    }
    return nullptr;
}

llvm::Value * RoutineBody::codeGen(CodeGenContext &context) {
    return compoundStmt->codeGen(context);
}

llvm::Value * CompoundStmt::codeGen(CodeGenContext &context) {
    return stmtList->codeGen(context);
}

llvm::Value * StmtList::codeGen(CodeGenContext &context) {
    preList->codeGen(context);
    stmt->codeGen(context);
    return nullptr;
}

llvm::Value * Stmt::codeGen(CodeGenContext &context) {
    nonLabelStmt->codeGen(context);
    return nullptr;
}

llvm::Value * NonLabelStmt::codeGen(CodeGenContext &context) {
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

llvm::Value * AssignStmt::codeGen(CodeGenContext &context) {
    CodeGenBlock * b = context.blocks.top();
    while (b) {
        if(b->locals.find(id) == b->locals.end()) b = b->preBlock;
        return new llvm::StoreInst(rhs->codeGen(context), b->locals[id], false, context.currentBlock());
    }
    return nullptr;
}

llvm::Value * IfStmt::codeGen(CodeGenContext &context) {
    Value * condition = expression->codeGen(context);
    BasicBlock *btrue = BasicBlock::Create(MyContext, "thenStmt", context.blocks.top()->function);
    BasicBlock *bfalse = BasicBlock::Create(MyContext, "elseStmt", context.blocks.top()->function);
    BasicBlock *bmerge = BasicBlock::Create(MyContext, "mergeStmt", context.blocks.top()->function);
    llvm::Instruction* ret = llvm::BranchInst::Create(btrue,bfalse,condition,context.currentBlock());
    context.pushBlock(btrue);

    stmt->codeGen(context);
    llvm::BranchInst::Create(bmerge,context.currentBlock());
    context.popBlock();
    context.pushBlock(bfalse);
    if(elseClause)
        elseClause->codeGen(context);
    llvm::BranchInst::Create(bmerge,context.currentBlock());
    context.popBlock();
    context.pushBlock(bmerge);
    context.popBlock();//??
    return ret;
}

llvm::Value * ElseClause::codeGen(CodeGenContext &context) {
    return stmt->codeGen(context);
}

llvm::Value * WhileStmt::codeGen(CodeGenContext &context) {
    BasicBlock *sloop = BasicBlock::Create(MyContext, "startloop", context.blocks.top()->function);
    BasicBlock *bloop = BasicBlock::Create(MyContext, "loopStmt", context.blocks.top()->function);
    BasicBlock *bexit = BasicBlock::Create(MyContext, "eixtStmt", context.blocks.top()->function);

    llvm::BranchInst::Create(sloop,context.currentBlock());
    context.pushBlock(sloop);
    Value* test = whileCondition->codeGen( context );
    llvm::Instruction *ret = llvm::BranchInst::Create(bloop,bexit,test,context.currentBlock());
    context.popBlock();
    context.pushBlock(bloop);
    stmt->codeGen(context);
    llvm::BranchInst::Create(sloop,context.currentBlock());
    context.popBlock();
    context.pushBlock(bexit);
    context.popBlock(); // ??
    return ret;
}

llvm::Value* RepeatStmt::codeGen(CodeGenContext& context) {
    BasicBlock *bloop = BasicBlock::Create(MyContext, "loopStmt", context.blocks.top()->function);
    BasicBlock *bexit = BasicBlock::Create(MyContext, "eixtStmt", context.blocks.top()->function);
    llvm::BranchInst::Create(bloop,context.currentBlock());

    context.pushBlock(bloop);
    stmtList->codeGen(context);
    Value* test = untilCondition->codeGen( context );
    llvm::Instruction *ret = llvm::BranchInst::Create(bexit,bloop,test,context.currentBlock());
    context.popBlock();

    context.pushBlock(bexit);
    context.popBlock(); // ??
    return ret;
}

llvm::Value* Expression::codeGen(CodeGenContext& context) {
    if(type == T_EXPR){
        return expr->codeGen(context);
    } else {
        Value * op1_val = expression->codeGen(context);
        Value * op2_val = expr->codeGen(context);
        switch (type) {
            case T_EQ:
                return llvm::CmpInst::Create( llvm::Instruction::ICmp, llvm::CmpInst::ICMP_EQ,
                                                    op1_val, op2_val, "", context.currentBlock());
            case T_NE:
                return  llvm::CmpInst::Create( llvm::Instruction::ICmp, llvm::CmpInst::ICMP_NE,
                                               op1_val, op2_val, "", context.currentBlock());
            case T_LT:
                return  llvm::CmpInst::Create( llvm::Instruction::ICmp, llvm::CmpInst::ICMP_SLT,
                                               op1_val, op2_val, "", context.currentBlock());
            case T_GT:
                return  llvm::CmpInst::Create( llvm::Instruction::ICmp, llvm::CmpInst::ICMP_SGT,
                                               op1_val, op2_val, "", context.currentBlock());
            case T_LE:
                return  llvm::CmpInst::Create( llvm::Instruction::ICmp, llvm::CmpInst::ICMP_SLE,
                                               op1_val, op2_val, "", context.currentBlock());
            case T_GE:
                return  llvm::CmpInst::Create( llvm::Instruction::ICmp, llvm::CmpInst::ICMP_SGE,
                                               op1_val, op2_val, "", context.currentBlock());
            default:
                return nullptr;
        }
    }
}

llvm::Value * Expr::codeGen(CodeGenContext &context) {
    if(type == T_TERM) return term->codeGen(context);
    Value * op1_val = expr->codeGen(context);
    Value * op2_val = term->codeGen(context);
    switch(type){
        case T_PLUS:
            return llvm::BinaryOperator::Create( llvm::Instruction::FAdd,
                                                 op1_val, op2_val, "", context.currentBlock());
        case T_MINUS:
            return llvm::BinaryOperator::Create( llvm::Instruction::FSub,
                                                 op1_val, op2_val, "", context.currentBlock());
        case T_OR:
            return llvm::BinaryOperator::Create( llvm::Instruction::Or,
                                                 op1_val, op2_val, "", context.currentBlock());
        default:
            return nullptr;
    }
}

llvm::Value * Term::codeGen(CodeGenContext &context) {
    if(type == T_FACTOR) return factor->codeGen(context);
    Value * op1_val = term->codeGen(context);
    Value * op2_val = factor->codeGen(context);
    switch(type) {
        case T_MUL:
            return llvm::BinaryOperator::Create( llvm::Instruction::FMul,
                                                 op1_val, op2_val, "", context.currentBlock());
        case T_DIV:
            return llvm::BinaryOperator::Create( llvm::Instruction::SDiv,
                                                 op1_val, op2_val, "", context.currentBlock());
        case T_AND:
            return llvm::BinaryOperator::Create( llvm::Instruction::And,
                                                 op1_val, op2_val, "", context.currentBlock());
        case T_MOD:
            return llvm::BinaryOperator::Create( llvm::Instruction::SRem,
                                                 op1_val, op2_val, "", context.currentBlock());
        default:
            return nullptr;
    }
}

llvm::Value * Factor::codeGen(CodeGenContext &context) {
    switch (type) {
        case T_NAME: {
            auto p = context.blocks.top();
            while (p) {
                if (p->locals.find(name) == p->locals.end()) p = p->preBlock;
                return new llvm::LoadInst(p->locals[name], "", false, context.currentBlock()); // ??
            }
            std::cout << "Undefined variable." << std::endl;
            return nullptr;
        }
        case T_CONST:
            return constValue->codeGen(context);
        case T_EXPR:
            return expression->codeGen(context);
        case T_NOT_FACTOR:
            return nullptr; // not finished
    }
}

llvm::Value* ForStmt::codeGen(CodeGenContext& context) {
    BasicBlock *sloop = BasicBlock::Create(MyContext, "startloop", context.blocks.top()->function);
    BasicBlock *bloop = BasicBlock::Create(MyContext, "loopStmt", context.blocks.top()->function);
    BasicBlock *bexit = BasicBlock::Create(MyContext, "eixtStmt", context.blocks.top()->function);
//  initial for
    AssignStmt* initial = new AssignStmt(loopId, firstBound);
    initial->codeGen(context);
    llvm::BranchInst::Create(sloop,context.currentBlock());
//  for test
    context.pushBlock(sloop);
    Factor * f = new Factor(Factor::T_NAME, loopId);
    Value* test = llvm::CmpInst::Create( llvm::Instruction::ICmp, llvm::CmpInst::ICMP_EQ,
                                         f->codeGen(context), secondBound->codeGen(context), "", context.currentBlock());
    llvm::Instruction *ret = llvm::BranchInst::Create(bexit,bloop,test,context.currentBlock());
    context.popBlock();

    context.pushBlock(bloop);
    stmt->codeGen(context);
//update
    Factor * f1;
    auto int1 = new ConstValue("1", ConstValue::T_INTEGER);
    Value * update;
    if (direction->type == Direction::T_TO) {
        f1 = new Factor(Factor::T_NAME, loopId);
        update = llvm::BinaryOperator::Create( llvm::Instruction::Add,
                                      f1->codeGen(context), int1->codeGen(context), "", context.currentBlock());
    }
    else{
        f1 = new Factor(Factor::T_NAME, loopId);
        update = llvm::BinaryOperator::Create( llvm::Instruction::Sub,
                                      f1->codeGen(context), int1->codeGen(context), "", context.currentBlock());
    }
    CodeGenBlock * b = context.blocks.top();
    while (b) {
        if(b->locals.find(loopId) == b->locals.end()) b = b->preBlock;
        new llvm::StoreInst(update, b->locals[loopId], false, context.currentBlock());
    }
    llvm::BranchInst::Create(sloop,context.currentBlock());
    context.popBlock();

    context.pushBlock(bexit);
    context.popBlock(); // ??
//    loopStmt->CodeGen(context); why ?
    delete initial;
    delete f;
    delete int1;
//    delete update;
    delete f1;
    return ret;
}
