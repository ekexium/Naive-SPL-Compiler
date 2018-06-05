/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SYS_CON = 258,
     SYS_FUNCT = 259,
     SYS_PROC = 260,
     SYS_TYPE = 261,
     ID = 262,
     INTEGER = 263,
     REAL = 264,
     LP = 265,
     RP = 266,
     LB = 267,
     RB = 268,
     DOT = 269,
     COMMA = 270,
     COLON = 271,
     ASSIGN = 272,
     DOTDOT = 273,
     SEMI = 274,
     ARRAY = 275,
     BEGIN = 276,
     CASE = 277,
     CONST = 278,
     DO = 279,
     DOWNTO = 280,
     ELSE = 281,
     END = 282,
     FOR = 283,
     FUNCTION = 284,
     GOTO = 285,
     IF = 286,
     IN = 287,
     OF = 288,
     PACKED = 289,
     PROCEDURE = 290,
     PROGRAM = 291,
     RECORD = 292,
     REPEAT = 293,
     SET = 294,
     THEN = 295,
     TO = 296,
     TYPE = 297,
     UNTIL = 298,
     VAR = 299,
     WHILE = 300,
     WITH = 301,
     NOT = 302,
     MUL = 303,
     DIV = 304,
     MOD = 305,
     AND = 306,
     PLUS = 307,
     MINUS = 308,
     OR = 309,
     XOR = 310,
     EQ = 311,
     NE = 312,
     GE = 313,
     GT = 314,
     LE = 315,
     LT = 316,
     READ = 317
   };
#endif
/* Tokens.  */
#define SYS_CON 258
#define SYS_FUNCT 259
#define SYS_PROC 260
#define SYS_TYPE 261
#define ID 262
#define INTEGER 263
#define REAL 264
#define LP 265
#define RP 266
#define LB 267
#define RB 268
#define DOT 269
#define COMMA 270
#define COLON 271
#define ASSIGN 272
#define DOTDOT 273
#define SEMI 274
#define ARRAY 275
#define BEGIN 276
#define CASE 277
#define CONST 278
#define DO 279
#define DOWNTO 280
#define ELSE 281
#define END 282
#define FOR 283
#define FUNCTION 284
#define GOTO 285
#define IF 286
#define IN 287
#define OF 288
#define PACKED 289
#define PROCEDURE 290
#define PROGRAM 291
#define RECORD 292
#define REPEAT 293
#define SET 294
#define THEN 295
#define TO 296
#define TYPE 297
#define UNTIL 298
#define VAR 299
#define WHILE 300
#define WITH 301
#define NOT 302
#define MUL 303
#define DIV 304
#define MOD 305
#define AND 306
#define PLUS 307
#define MINUS 308
#define OR 309
#define XOR 310
#define EQ 311
#define NE 312
#define GE 313
#define GT 314
#define LE 315
#define LT 316
#define READ 317




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 10 "parser.y"
{
    Node *node;
    Program *program;
    ProgramHead *programHead;
    Routine *routine;
    RoutineHead *routineHead;
    SubRoutine *subRoutine;
    LabelPart *labelPart;
    ConstPart *constPart;
    ConstExprList *constExprList;
    ConstValue *constValue;
    TypePart *typePart;
    TypeDeclList *typeDeclList;
    TypeDefinition *typeDefinition;
    TypeDecl *typeDecl;
    SimpleTypeDecl *simpleTypeDecl;
    ArrayTypeDecl *arrayTypeDecl;
    RecordTypeDecl *recordTypeDecl;
    FieldDeclList *fieldDeclList;
    FieldDecl *fieldDecl;
    NameList *nameList;
    VarPart *varPart;
    VarDeclList *varDeclList;
    VarDecl *varDecl;
    RoutinePart *routinePart;
    FunctionDecl *functionDecl;
    FunctionHead *functionHead;
    ProcedureDecl *procedureDecl;
    ProcedureHead *procedureHead;
    Parameters *parameters;
    ParaDeclList *paraDeclList;
    ParaTypeList *paraTypeList;
    VarParaList *varParaList;
    ValParaList *valParaList;
    RoutineBody *routineBody;
    CompoundStmt *compoundStmt;
    StmtList *stmtList;
    Stmt *stmt;
    NonLabelStmt *nonLabelStmt;
    AssignStmt *assignStmt;
    ProcStmt *procStmt;
    IfStmt *ifStmt;
    ElseClause *elseClause;
    RepeatStmt *repeatStmt;
    WhileStmt *whileStmt;
    ForStmt *forStmt;
    Direction *direction;
    CaseStmt *caseStmt;
    CaseExprList *caseExprList;
    CaseExpr *caseExpr;
    GotoStmt *gotoStmt;
    ExpressionList *expressionList;
    Expression *expression;
    Expr *expr;
    Term *term;
    Factor *factor;
    ArgsList *argsList;

    AbstractStatement *abstractStatement;
    AbstractExpression *abstractExpression;

    std::string *string;
    int token;
}
/* Line 1529 of yacc.c.  */
#line 238 "parser.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

