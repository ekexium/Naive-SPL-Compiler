//
// Created by QZQ on /5/30.
//

#ifndef SPLC_NODE_H
#define SPLC_NODE_H

#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>
#include "NodePredeclaration.h"

class CodeGenContext;

class Node {
public:
	virtual ~Node() = default;

	virtual llvm::Value *codeGen(CodeGenContext &context) {}
};

class AbstractExpression : public Node {
};

class AbstractStatement : public Node {
};

class Program : public AbstractStatement {
public:

	ProgramHead *programHead{};
	Routine *routine{};
};

class ProgramHead : public AbstractStatement {
public:
	std::string id;
};


class Routine : public AbstractStatement {
public:

	RoutineHead *routineHead{};
	RoutineBody *routineBody{};
};

class SubRoutine : public AbstractStatement {
public:

	RoutineHead *routineHead{};
	RoutineBody *routineBody{};
};

class RoutineHead : public AbstractStatement {
public:

	LabelPart *labelPart{};

};

class LabelPart : public AbstractStatement {
public:


};

class ConstPart : public AbstractStatement {
public:

	ConstExprList *constExprList{};
};

class ConstExprList : public AbstractStatement {
public:
	ConstExprList *preList{};
	ConstValue *value{};
};

class ConstValue : public AbstractExpression {
public:
	static const int INTEGER = 1;
	static const int REAL = 2;
	static const int CHAR = 3;
	static const int SYS_CON = 4;
	//todo: does string exist?
	static const int STRING = 5;

	std::string value;
	int type{};

	ConstValue(std::string &value, int type) : value(value), type(type) {}
};

class TypePart : public AbstractStatement {
public:
	TypeDeclList *typeDeclList{};
};

class TypeDeclList : public AbstractStatement {
public:
	TypeDeclList *preList{};
	TypeDefinition *typeDefinition{};
};

class TypeDefinition : public AbstractStatement {
public:
	TypeDecl *typeDecl{};
};

class TypeDecl : public AbstractStatement {
public:
	static const int SIMPLE_TYPE_DECLARE = 1;
	static const int ARRAY_TYPE_DECLARE = 2;
	static const int RECORD_TYPE_DECLARE = 3;

	int type{};
	SimpleTypeDecl *simpleTypeDecl{};
	ArrayTypeDecl *arrayTypeDecl{};
	RecordTypeDecl *recordTypeDecl{};
};

class SimpleTypeDecl : public AbstractStatement {
public:
	static const int SYS_TYPE = 1;
	static const int TYPE_NAME = 2;
	static const int ENUMERATION = 3;
	static const int SUBRAGE = 4;

	int type{};
	std::string sysType;
	std::string name;
	//todo: manually handle negative signs?
	int lowerBound, upperBound;

};

class ArrayTypeDecl : public AbstractStatement {
public:
	SimpleTypeDecl *range{};
	TypeDecl *elementType{};
};

class RecordTypeDecl : public AbstractStatement {
public:
	FieldDeclList *fieldDeclList{};
};

class FieldDeclList : public AbstractStatement {
public:
	FieldDeclList *preList{};
	FieldDecl *fieldDecl{};
};

class FieldDecl : public AbstractStatement {
public:
	NameList *nameList{};
	TypeDecl *typeDecl{};
};

class NameList : public AbstractStatement {
public:
	NameList *nameList;
	std::string name;
};

class VarPart : public AbstractStatement {
public:
	VarDeclList *varDeclList{};
};

class VarDeclList : public AbstractStatement {
public:
	VarDeclList *preList{};
	VarDecl *varDecl{};
};

class VarDecl : public AbstractStatement {
public:
	NameList *nameList{};
	TypeDecl *typeDecl{};
};

class RoutinePart : public AbstractStatement {
public:
	//todo: don't know what it is
	static const int ROUTINE_FUNC = 1;
	static const int ROUTINE_PROC = 2;
	static const int FUNC = 3;
	static const int PROC = 4;
	static const int EMPTY = 5;

	int type{};
	RoutinePart *routinePart{};
	FunctionDecl *functionDecl{};
	ProcedureDecl *procedureDecl{};
};

class FunctionDecl : public AbstractStatement {
public:
	FunctionHead *functionHead{};
	SubRoutine *subRoutine{};
};

class FunctionHead : public AbstractStatement {
public:
	std::string name;
	Parameters *parameters{};
	SimpleTypeDecl *returnType{};
};

class ProcedureDecl : public AbstractStatement {
public:
	ProcedureHead *procedureHead{};
	SubRoutine *subRoutine{};
};

class ProcedureHead : public AbstractStatement {
public:
	std::string name;
	Parameters *parameters;
};

class Parameters : public AbstractStatement {
public:
	ParaDeclList *paraDeclList{};
};

class ParaDeclList : public AbstractStatement {
public:
	ParaDeclList *paraDeclList{};
	ParaTypeList *paraTypeList{};
};

class ParaTypeList : public AbstractStatement {
public:
	static const int VAR = 1;
	static const int VAL = 2;

	int type{};
	VarParaList *varParaList{};
	ValParaList *valParaList{};
	SimpleTypeDecl *typeDecl{};
};

class VarParaList : public AbstractStatement {
public:
	NameList *nameList{};
};

class ValParaList : public AbstractStatement {
public:
	NameList *nameList{};
};

class RoutineBody : public AbstractStatement {
public:
	CompoundStmt *compoundStmt{};
};

class CompoundStmt : public AbstractStatement {
public:
	StmtList *stmtList{};
};

class StmtList : public AbstractStatement {
public:
	StmtList *preList{};
	Stmt *stmt{};
};

class Stmt : public AbstractStatement {
public:
	static const int LABELED = 1;
	static const int UNLABELED = 2;

	int type{};
	NonLabelStmt *nonLabelStmt{};
};

class NonLabelStmt : public AbstractStatement {
public:
	static const int ASSIGN = 1;
	static const int PROC = 2;
	static const int IF = 3;
	static const int REPEAT = 4;
	static const int WHILE = 5;
	static const int FOR = 6;
	static const int CASE = 7;
	static const int GOTO = 8;

	int type{};
	AssignStmt *assignStmt{};
	ProcStmt *procStmt{};
	IfStmt *ifStmt{};
	RepeatStmt *repeatStmt{};
	WhileStmt *whileStmt{};
	ForStmt *forStmt{};
	CaseStmt *caseStmt{};
	GotoStmt *gotoStmt{};
};

class AssignStmt : public AbstractStatement {
public:
	static const int SIMPLE = 1;
	static const int ARRAY = 2;
	static const int RECORD = 3;
	std::string id;
	Expression *rhs;
	Expression *index;
	std::string recordId;
};

class ProcStmt : public AbstractStatement {
public:
	static const int SIMPLE = 1;
	static const int SIMPLE_ARGS = 2;
	static const int SYS_PROC = 3;
	static const int SYS_PROC_EXPR = 4;
	//fixme: what's read ??
	static const int READ = 5;

	int type{};
	std::string procId;
	ArgsList *argsList{};
	std::string sysProc;
	ExpressionList *expressionList{};

	//fixme: read(factor) ??
	Factor *factor{};
};

class IfStmt : public AbstractStatement {
public:
	Expression *expression{};
	Stmt *stmt{};
	ElseClause *elseClause{};
};

class ElseClause : public AbstractStatement {
public:
	Stmt *stmt{};
};

class RepeatStmt : public AbstractStatement {
public:
	StmtList *stmtList{};
	Expression *untilCondition{};
};

class WhileStmt : public AbstractStatement {
public:
	Expression *whileCondition{};
	Stmt *stmt{};
};

class ForStmt : public AbstractStatement {
public:
	std::string loopId;
	Expression *lowerBound;
	Direction *direction;
	Expression *upperBound;
	Stmt *stmt;
};

class Direction : public AbstractStatement {
public:
	static const int TO = 1;
	static const int DOWNTO = 2;
	int type{};
};

class CaseStmt : public AbstractStatement {
public:
	Expression *expression{};
	CaseExprList *caseExprList{};
};

class CaseExprList : public AbstractStatement {
public:
	CaseExprList *preList{};
	CaseExpr caseExpr;
};

class CaseExpr : public AbstractStatement {
public:
	static const int CONST = 1;
	static const int ID = 2;
	ConstValue *constValue;
	std::string id;
	Stmt *stmt;
};

class GotoStmt : public AbstractStatement {
public:
	int addr{};
};

// todo: expression or statement, does it matter?
class ExpressionList : AbstractExpression {
public:
	ExpressionList *preList{};
	Expression *expression{};
};

class Expression : public AbstractExpression {
public:
	static const int EQ = 1;
	static const int NE = 2;
	static const int GE = 3;
	static const int GT = 4;
	static const int LE = 5;
	static const int LT = 6;
	static const int EXPR = 7;
	Expression *expression{};
	Expr *expr{};
};

class Expr : public AbstractExpression {
public:
	static const int PLUS = 1;
	static const int MINUS = 2;
	static const int OR = 3;
	static const int TERM = 4;

	Expr *expr{};
	Term *term{};
};

class Term : public AbstractExpression {
public:
	static const int MUL = 1;
	static const int DIV = 2;
	static const int MOD = 3;
	static const int AND = 4;
	static const int FACTOR = 5;

	Term *term{};
	Factor *factor{};
};

class Factor : public AbstractExpression {
public:
	static const int NAME = 1;
	static const int NAME_ARGS = 2;
	static const int SYS_FUNCT = 3;
	static const int SYS_FUNCT_ARGS = 4;
	static const int CONST = 5;
	static const int EXPR = 6;
	static const int NOT_FACTOR = 7;
	static const int MINUS_FACTOR = 8;
	static const int ID_EXPR = 9;
	static const int ID_DOT_ID = 10;

	int type{};
	std::string name;
	ArgsList *argsList{};
	std::string sysFunction;
	ConstValue *constValue{};
	Expression *expression{};
	Factor *factor{};
	std::string id;
	std::string recordId;
};

class ArgsList : public AbstractStatement {
public:
	ArgsList *preList{};
	Expression *expression{};
};

#endif //SPLC_NODE_H
