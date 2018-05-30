//
// Created by QZQ on /5/30.
//

#ifndef SPLC_NODE_H
#define SPLC_NODE_H

#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

class CodeGenContext;

class Node {
public:
	virtual ~Node() {}

	virtual llvm::Value *codeGen(CodeGenContext &context) {}
};

class AbstractExpression : public Node {
};

class AbstractStatement : public Node {
};

class Program : public AbstractStatement {

};

class ProgramHead : public AbstractStatement {

};

class Routine : public AbstractStatement {

};

class SubRoutine : public AbstractStatement {

};

class RoutineHead : public AbstractStatement {

};

class LabelPart : public AbstractStatement {

};

class ConstPart : public AbstractStatement {

};

class ConstExprList : public AbstractStatement {

};

class ConstValue : public AbstractExpression {

};

class TypePart : public AbstractStatement {

};

class TypeDeclList : public AbstractStatement {

};

class TypeDefinition : public AbstractStatement {

};

class TypeDecl : public AbstractStatement {

};

class SimpleTypeDecl : public AbstractStatement {

};

class ArrayTypeDecl : public AbstractStatement {

};

class RecordTypeDecl : public AbstractStatement {

};

class FieldDeclList : public AbstractStatement {

};

class FieldDecl : public AbstractStatement {
};

class NameList : public AbstractStatement {
};

class VarPart : public AbstractStatement {
};

class VarDeclList : public AbstractStatement {
};

class VarDecl : public AbstractStatement {
};

class RoutinePart : public AbstractStatement {
};

class FunctionDecl : public AbstractStatement {
};

class FunctionHead : public AbstractStatement {
};

class ProcedureDecl : public AbstractStatement {
};

class ProcedureHead : public AbstractStatement {
};

class Parameters : public AbstractStatement {
};

class ParaDeclList : public AbstractStatement {
};

class ParaTypeList : public AbstractStatement {
};

class VarParaList : public AbstractStatement {
};

class ValParaList : public AbstractStatement {
};

class RountineBody : public AbstractStatement {
};

class CompoundStmt : public AbstractStatement {
};

class StmtList : public AbstractStatement {
};

class Statement : public AbstractStatement {
};

class NonLabelStmt : public AbstractStatement {
};

class AssignStmt : public AbstractStatement {
};

class ProcStmt : public AbstractStatement {
};

class IfStmt : public AbstractStatement {
};

class ElseClause : public AbstractStatement {
};

class RepeatStmt : public AbstractStatement {
};

class WhileStmt : public AbstractStatement {
};

class ForStmt : public AbstractStatement {
};

class Direction : public AbstractStatement {
};

class CaseStmt : public AbstractStatement {
};

class CaseExprList : public AbstractExpression {
};

class GotoStmt : public AbstractStatement {
};

// todo: expression or statement, does it matter?
class ExpressionList : AbstractExpression {
};

class Expression : public AbstractExpression {};

class Expr : public AbstractExpression {};

class Term : public AbstractExpression {};

class Factor : public AbstractExpression {};

class SysFunct : public AbstractExpression {};

class ArgsList : public AbstractStatement {};

#endif //SPLC_NODE_H
