//
// Created by QZQ on /5/30.
//

#ifndef SPLC_NODE_H
#define SPLC_NODE_H

#include <iostream>
#include <map>
#include <utility>
#include <llvm/IR/Value.h>
#include "NodePredeclaration.h"
#include "codegen.h"

class Node {
public:
	virtual ~Node() = default;

	virtual llvm::Value *codeGen(CodeGenContext &context) = 0;
};

class AbstractExpression : public Node {
};

class AbstractStatement : public Node {
};

class Program : public AbstractStatement {
public:

	ProgramHead *programHead{};
	Routine *routine{};

	Program(ProgramHead *programHead, Routine *routine) : programHead(programHead), routine(routine) {}
};

class ProgramHead : public AbstractStatement {
public:
	std::string id;

	explicit ProgramHead(std::string id) : id(std::move(id)) {}
};


class Routine : public AbstractStatement {
public:

	RoutineHead *routineHead{};
	RoutineBody *routineBody{};

	Routine(RoutineHead *routineHead, RoutineBody *routineBody) : routineHead(routineHead), routineBody(routineBody) {}

};

class SubRoutine : public AbstractStatement {
public:

	RoutineHead *routineHead{};
	RoutineBody *routineBody{};

	SubRoutine(RoutineHead *routineHead, RoutineBody *routineBody) : routineHead(routineHead),
																	 routineBody(routineBody) {}
};

class RoutineHead : public AbstractStatement {
public:

	LabelPart *labelPart;
	ConstPart *constPart;
	TypePart *typePart;
	VarPart *varPart;
	RoutinePart *routinePart;

	RoutineHead(LabelPart *labelPart, ConstPart *constPart, TypePart *typePart, VarPart *varPart,
				RoutinePart *routinePart) : labelPart(labelPart), constPart(constPart), typePart(typePart),
											varPart(varPart), routinePart(routinePart) {}
};

class LabelPart : public AbstractStatement {
public:

};

class ConstPart : public AbstractStatement {
public:

	ConstExprList *constExprList{};

	explicit ConstPart(ConstExprList *constExprList) : constExprList(constExprList) {}

};

class ConstExprList : public AbstractStatement {
public:

	std::string name;
	ConstExprList *preList{};
	ConstValue *value{};

	ConstExprList(std::string name, ConstExprList *preList, ConstValue *value) : name(std::move(name)),
																				 preList(preList),
																				 value(value) {}
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

	ConstValue(std::string value, int type) : value(std::move(value)), type(type) { assert(type >= 1 && type <= 5); }

	ConstValue *negate() {
		if (value == "-" || value.empty())
			return new ConstValue("", type);
		if (value[0] == '-')
			return new ConstValue(value.substr(1), type);
		else
			return new ConstValue("-" + value, type);
	}
};

class TypePart : public AbstractStatement {
public:

	TypeDeclList *typeDeclList{};

	explicit TypePart(TypeDeclList *typeDeclList) : typeDeclList(typeDeclList) {}

};

class TypeDeclList : public AbstractStatement {
public:
	TypeDeclList *preList{};
	TypeDefinition *typeDefinition{};

	TypeDeclList(TypeDeclList *preList, TypeDefinition *typeDefinition) : preList(preList),
																		  typeDefinition(typeDefinition) {}

};

class TypeDefinition : public AbstractStatement {
public:
	std::string name;
	TypeDecl *typeDecl{};

	TypeDefinition(std::string name, TypeDecl *typeDecl) : name(std::move(name)), typeDecl(typeDecl) {}
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

	explicit TypeDecl(SimpleTypeDecl *simpleTypeDecl) : simpleTypeDecl(simpleTypeDecl) { type = SIMPLE_TYPE_DECLARE; }

	explicit TypeDecl(ArrayTypeDecl *arrayTypeDecl) : arrayTypeDecl(arrayTypeDecl) { type = ARRAY_TYPE_DECLARE; }

	explicit TypeDecl(RecordTypeDecl *recordTypeDecl) : recordTypeDecl(recordTypeDecl) { type = RECORD_TYPE_DECLARE; }
};

class SimpleTypeDecl : public AbstractStatement {
public:
	static const int SYS_TYPE = 1;
	static const int TYPE_NAME = 2;
	static const int ENUMERATION = 3;
	static const int RANGE = 4;
	static const int NAME_RANGE = 5;

	int type{};
	std::string sysType;
	std::string name;
	NameList *nameList{};
	//todo: manually handle negative signs?
	ConstValue *lowerBound{}, *upperBound{};
	std::string lowerName, upperName;

	SimpleTypeDecl(int type, const std::string &st) : type(type) {
		if (type == SYS_TYPE)
			sysType = st;
		if (type == TYPE_NAME)
			name = st;
		assert(type == SYS_TYPE || type == TYPE_NAME);
	}

	explicit SimpleTypeDecl(NameList *nameList) : type(type), nameList(nameList), type(ENUMERATION) {}

	SimpleTypeDecl(ConstValue *lowerBound, ConstValue *upperBound) : lowerBound(lowerBound),
																	 upperBound(upperBound), type(RANGE) {}

	SimpleTypeDecl(std::string lowerName, std::string upperName) : lowerName(std::move(lowerName)),
																   upperName(std::move(upperName)), type(NAME_RANGE) {}
};

class ArrayTypeDecl : public AbstractStatement {
public:
	SimpleTypeDecl *range{};
	TypeDecl *elementType{};

	ArrayTypeDecl(SimpleTypeDecl *range, TypeDecl *elementType) : range(range), elementType(elementType) {}
};

class RecordTypeDecl : public AbstractStatement {
public:
	FieldDeclList *fieldDeclList{};

	explicit RecordTypeDecl(FieldDeclList *fieldDeclList) : fieldDeclList(fieldDeclList) {}
};

class FieldDeclList : public AbstractStatement {
public:
	FieldDeclList *preList{};
	FieldDecl *fieldDecl{};

	FieldDeclList(FieldDeclList *preList, FieldDecl *fieldDecl) : preList(preList), fieldDecl(fieldDecl) {}
};

class FieldDecl : public AbstractStatement {
public:
	NameList *nameList{};
	TypeDecl *typeDecl{};

	FieldDecl(NameList *nameList, TypeDecl *typeDecl) : nameList(nameList), typeDecl(typeDecl) {}
};

class NameList : public AbstractStatement {
public:
	NameList *nameList;
	std::string name;

	NameList(NameList *nameList, std::string name) : nameList(nameList), name(std::move(name)) {}
};

class VarPart : public AbstractStatement {
public:
	VarDeclList *varDeclList{};

	explicit VarPart(VarDeclList *varDeclList) : varDeclList(varDeclList) {}
};

class VarDeclList : public AbstractStatement {
public:
	VarDeclList *preList{};
	VarDecl *varDecl{};

	VarDeclList(VarDeclList *preList, VarDecl *varDecl) : preList(preList), varDecl(varDecl) {}
};

class VarDecl : public AbstractStatement {
public:
	NameList *nameList{};
	TypeDecl *typeDecl{};

	VarDecl(NameList *nameList, TypeDecl *typeDecl) : nameList(nameList), typeDecl(typeDecl) {}
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

	RoutinePart(RoutinePart *routinePart, FunctionDecl *functionDecl) : routinePart(routinePart),
																		functionDecl(
																				functionDecl) { type = ROUTINE_FUNC; }

	RoutinePart(RoutinePart *routinePart, ProcedureDecl *procedureDecl) : routinePart(routinePart),
																		  procedureDecl(
																				  procedureDecl) { type = ROUTINE_PROC; }

	explicit RoutinePart(FunctionDecl *functionDecl) : functionDecl(functionDecl) { type = FUNC; }

	explicit RoutinePart(ProcedureDecl *procedureDecl) : procedureDecl(procedureDecl) { type = PROC; }

	explicit RoutinePart(int type) : type(type) { assert(type == EMPTY); }
};

class FunctionDecl : public AbstractStatement {
public:
	FunctionHead *functionHead{};
	SubRoutine *subRoutine{};

	FunctionDecl(FunctionHead *functionHead, SubRoutine *subRoutine) : functionHead(functionHead),
																	   subRoutine(subRoutine) {}
};

class FunctionHead : public AbstractStatement {
public:
	std::string name;
	Parameters *parameters{};
	SimpleTypeDecl *returnType{};

	FunctionHead(std::string name, Parameters *parameters, SimpleTypeDecl *returnType) : name(std::move(name)),
																						 parameters(parameters),
																						 returnType(
																								 returnType) {}
};

class ProcedureDecl : public AbstractStatement {
public:
	ProcedureHead *procedureHead{};
	SubRoutine *subRoutine{};

	ProcedureDecl(ProcedureHead *procedureHead, SubRoutine *subRoutine) : procedureHead(procedureHead),
																		  subRoutine(subRoutine) {}
};

class ProcedureHead : public AbstractStatement {
public:
	std::string name;
	Parameters *parameters;

	ProcedureHead(std::string name, Parameters *parameters) : name(std::move(name)), parameters(parameters) {}
};

class Parameters : public AbstractStatement {
public:
	ParaDeclList *paraDeclList{};

	explicit Parameters(ParaDeclList *paraDeclList) : paraDeclList(paraDeclList) {}
};

class ParaDeclList : public AbstractStatement {
public:
	ParaDeclList *paraDeclList{};
	ParaTypeList *paraTypeList{};

	ParaDeclList(ParaDeclList *paraDeclList, ParaTypeList *paraTypeList) : paraDeclList(paraDeclList),
																		   paraTypeList(paraTypeList) {}
};

class ParaTypeList : public AbstractStatement {
public:
	static const int VAR = 1;
	static const int VAL = 2;

	int type{};
	VarParaList *varParaList{};
	ValParaList *valParaList{};
	SimpleTypeDecl *typeDecl{};

	ParaTypeList(VarParaList *varParaList, SimpleTypeDecl *typeDecl) : varParaList(varParaList),
																	   typeDecl(typeDecl) { type = VAR; }

	ParaTypeList(ValParaList *valParaList, SimpleTypeDecl *typeDecl) : valParaList(valParaList),
																	   typeDecl(typeDecl) { type = VAL; }
};

class VarParaList : public AbstractStatement {
public:
	NameList *nameList{};

	explicit VarParaList(NameList *nameList) : nameList(nameList) {}
};

class ValParaList : public AbstractStatement {
public:
	NameList *nameList{};

	explicit ValParaList(NameList *nameList) : nameList(nameList) {}
};

class RoutineBody : public AbstractStatement {
public:
	CompoundStmt *compoundStmt{};

	explicit RoutineBody(CompoundStmt *compoundStmt) : compoundStmt(compoundStmt) {}
};

class CompoundStmt : public AbstractStatement {
public:
	StmtList *stmtList{};

	explicit CompoundStmt(StmtList *stmtList) : stmtList(stmtList) {}
};

class StmtList : public AbstractStatement {
public:
	StmtList *preList{};
	Stmt *stmt{}; // NOTICE: fixme: might be nullptr, due to the grammar given...

	StmtList(StmtList *preList, Stmt *stmt) : preList(preList), stmt(stmt) {}
};

class Stmt : public AbstractStatement {
public:
	static const int LABELED = 1;
	static const int UNLABELED = 2;

	int type{};
	NonLabelStmt *nonLabelStmt{};

	Stmt(int type, NonLabelStmt *nonLabelStmt) : type(type), nonLabelStmt(nonLabelStmt) {
		assert(type == LABELED || type == UNLABELED);
	}
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

	explicit NonLabelStmt(AssignStmt *assignStmt) : assignStmt(assignStmt) { type = ASSIGN; }

	explicit NonLabelStmt(ProcStmt *procStmt) : procStmt(procStmt) { type = PROC; }

	explicit NonLabelStmt(IfStmt *ifStmt) : ifStmt(ifStmt) { type = IF; }

	explicit NonLabelStmt(RepeatStmt *repeatStmt) : repeatStmt(repeatStmt) { type = REPEAT; }

	explicit NonLabelStmt(WhileStmt *whileStmt) : whileStmt(whileStmt) { type = WHILE; }

	explicit NonLabelStmt(ForStmt *forStmt) : forStmt(forStmt) { type = FOR; }

	explicit NonLabelStmt(CaseStmt *caseStmt) : caseStmt(caseStmt) { type = CASE; }

	explicit NonLabelStmt(GotoStmt *gotoStmt) : gotoStmt(gotoStmt) { type = GOTO; }
};

class AssignStmt : public AbstractStatement {
public:
	static const int SIMPLE = 1;
	static const int ARRAY = 2;
	static const int RECORD = 3;

	int type{};
	std::string id;
	Expression *rhs;
	Expression *index{};
	std::string recordId;

	AssignStmt(std::string id, Expression *rhs) : id(std::move(id)), rhs(rhs), type(SIMPLE) {}

	AssignStmt(std::string id, Expression *index, Expression *rhs) : id(std::move(id)), rhs(rhs), index(index),
																	 type(ARRAY) {}

	AssignStmt(std::string id, std::string recordId, Expression *rhs) : id(std::move(id)), rhs(rhs),
																		recordId(std::move(recordId)),
																		type(RECORD) {}
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

	ProcStmt(int type, const std::string &st) : type(type) {
		assert(type == SIMPLE || type == SYS_PROC);
		if (type == SIMPLE)
			procId = st;
		else
			sysProc = st;
	}

	ProcStmt(std::string procId, ArgsList *argsList) : procId(std::move(procId)), argsList(argsList),
													   type(SIMPLE_ARGS) {}

	ProcStmt(std::string sysProc, ExpressionList *expressionList) : sysProc(std::move(sysProc)),
																	expressionList(expressionList),
																	type(SYS_PROC_EXPR) {}

	explicit ProcStmt(Factor *factor) : factor(factor), type(READ) {}

};

class IfStmt : public AbstractStatement {
public:
	Expression *expression{};
	Stmt *stmt{};
	ElseClause *elseClause{};

	IfStmt(Expression *expression, Stmt *stmt, ElseClause *elseClause) : expression(expression), stmt(stmt),
																		 elseClause(elseClause) {}
};

class ElseClause : public AbstractStatement {
public:
	Stmt *stmt{};

	explicit ElseClause(Stmt *stmt) : stmt(stmt) {}
};

class RepeatStmt : public AbstractStatement {
public:
	StmtList *stmtList{};
	Expression *untilCondition{};

	RepeatStmt(StmtList *stmtList, Expression *untilCondition) : stmtList(stmtList), untilCondition(untilCondition) {}
};

class WhileStmt : public AbstractStatement {
public:
	Expression *whileCondition{};
	Stmt *stmt{};

	WhileStmt(Expression *whileCondition, Stmt *stmt) : whileCondition(whileCondition), stmt(stmt) {}
};

class ForStmt : public AbstractStatement {
public:
	std::string loopId;
	Expression *firstBound;
	Direction *direction;
	Expression *secondBound;
	Stmt *stmt;

	ForStmt(std::string loopId, Expression *firstBound, Direction *direction, Expression *secondBound, Stmt *stmt)
			: loopId(std::move(loopId)), firstBound(firstBound), direction(direction), secondBound(secondBound),
			  stmt(stmt) {}
};

class Direction : public AbstractStatement {
public:
	static const int TO = 1;
	static const int DOWNTO = 2;
	int type{};

	explicit Direction(int type) : type(type) { assert(type == TO || type == DOWNTO); }
};

class CaseStmt : public AbstractStatement {
public:
	Expression *expression{};
	CaseExprList *caseExprList{};

	CaseStmt(Expression *expression, CaseExprList *caseExprList) : expression(expression), caseExprList(caseExprList) {}
};

class CaseExpr : public AbstractStatement {
public:
	static const int CONST = 1;
	static const int ID = 2;
	int type{};
	ConstValue *constValue{};
	std::string id;
	Stmt *stmt;

//	CaseExpr(){}

//	CaseExpr(const CaseExpr & caseExpr) : constValue(caseExpr.constValue), stmt(caseExpr.stmt), type(caseExpr.type) {}

	CaseExpr(ConstValue *constValue, Stmt *stmt) : constValue(constValue), stmt(stmt), type(CONST) {}

	CaseExpr(std::string id, Stmt *stmt) : id(std::move(id)), stmt(stmt), type(ID) {}
};

class CaseExprList : public AbstractStatement {
public:
	CaseExprList *preList{};
	CaseExpr caseExpr;

	CaseExprList(CaseExprList *preList, const CaseExpr &caseExpr) : preList(preList), caseExpr(caseExpr) {}
};

class GotoStmt : public AbstractStatement {
public:
	int address{};

	explicit GotoStmt(int addr) : address(addr) {}
};

// todo: expression or statement, does it matter?
class ExpressionList : AbstractExpression {
public:
	ExpressionList *preList{};
	Expression *expression{};

	ExpressionList(ExpressionList *preList, Expression *expression) : preList(preList), expression(expression) {}
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
	int type;
	Expression *expression{};
	Expr *expr{};

	Expression(int type, Expression *expression, Expr *expr) : type(type), expression(expression), expr(expr) {
		assert(type >= 1 && type <= 6);
	}

	explicit Expression(Expr *expr) : expr(expr), type(EXPR) {}
};

class Expr : public AbstractExpression {
public:
	static const int PLUS = 1;
	static const int MINUS = 2;
	static const int OR = 3;
	static const int TERM = 4;

	int type;
	Expr *expr{};
	Term *term{};

	Expr(int type, Expr *expr, Term *term) : type(type), expr(expr), term(term) {
		assert(type >= 1 && type <= 3);
	}

	explicit Expr(Term *term) : term(term), type(TERM) {}
};

class Term : public AbstractExpression {
public:
	static const int MUL = 1;
	static const int DIV = 2;
	static const int MOD = 3;
	static const int AND = 4;
	static const int FACTOR = 5;

	int type;
	Term *term{};
	Factor *factor{};

	Term(int type, Term *term, Factor *factor) : type(type), term(term), factor(factor) {
		assert(type >= 1 && type <= 4);
	}

	explicit Term(Factor *factor) : factor(factor), type(FACTOR) {}
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

	int type;
	std::string name;
	ArgsList *argsList{};
	std::string sysFunction;
	ConstValue *constValue{};
	Expression *expression{};
	Factor *factor{};
	std::string id;
	std::string recordId;

	Factor(int type, const std::string &st) : type(type) {
		assert(type == NAME || type == SYS_FUNCT);
		if (type == NAME)
			name = st;
		else
			sysFunction = st;
	}

	Factor(int type, const std::string &st, ArgsList *argsList) : type(type), argsList(argsList) {
		assert(type == NAME_ARGS || type == SYS_FUNCT_ARGS);
		if (type == NAME_ARGS)
			name = st;
		else
			sysFunction = st;
	}

	explicit Factor(ConstValue *constValue) : constValue(constValue), type(CONST) {}

	explicit Factor(Expression *expression) : expression(expression), type(EXPR) {}

	Factor(int type, Factor *factor) : type(type), factor(factor) {
		assert(type == NOT_FACTOR || type == MINUS_FACTOR);
	}

	Factor(std::string id, Expression *expression) : expression(expression), id(std::move(id)), type(ID_EXPR) {}

	Factor(std::string id, std::string recordId) : id(std::move(id)), recordId(std::move(recordId)), type(ID_DOT_ID) {}
};

class ArgsList : public AbstractStatement {
public:
	ArgsList *preList;
	Expression *expression;

	ArgsList(ArgsList *preList, Expression *expression) : preList(preList), expression(expression) {}
};

#endif //SPLC_NODE_H
