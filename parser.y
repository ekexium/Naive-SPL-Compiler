%{
    #include "ASTNode.h"
    ProgramHead *program_head;

    extern int yylex();
    void yyerror(const char *s) { printf("ERROR: %s/n", s); }
%}

/* Represents the many different ways we can access our data */
%union {
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
    CompoundStmt *compountStmt;
    StmtList *stmtList;
    Stmt *stmt;
    NonLabelStmt *nonLabelStmt;
    AssignStmt *assignStmt;
    ProcStme *procStmt;
    IfStmt *ifStmt;
    ElseClause *elseClause;
    RepeatStmt *repeatStmt;
    WhileStme *whileStmt;
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

//     int integer;
//     double double;
    std::string *string;
    int token;
}

%token <token> SYS_CON SYS_FUNCT SYS_PROC SYS_TYPE 
%token <token> LP RP LB RB DOT COMMA COLON
%token <token> ASSIGN DOTDOT SEMI AND  ARRAY BEGIN CASE
%token <token> CONST DO DOWNTO ELSE END FOR FUNCTION
%token <token> GOTO IF IN MOD NOT OF PACKED PROCECURE
%token <token> PROGRAM RECORD REPEAT SET THEN TO TYPE
%token <token> UNTIL VAR WHILE WITH
%token <token> NOT
%token <token> MUL DIV MOD AND
%token <token> PLUS MINUS OR XOR
%token <token> EQ NE GE GT LE LT 
%token <token> ID INTEGER DOUBLE

%type <program> program
%type <programHead> program_head
%type <routine> routine
%type <routineHead> routine_head
%type <subRoutine> sub_routine
%type <labelPart> label_part
%type <constPart> const_part
%type <constExprList> const_expr_list
%type <constValue> const_value
%type <typePart> type_part
%type <typeDeclList> type_decl_list
%type <typeDefinition> type_definition
%type <typeDecl> type_decl
%type <simpleTypeDecl> simple_type_decl
%type <arrayTypeDecl> array_type_decl
%type <recordTypeDecl> record_type_decl
%type <fieldDeclList> field_decl_list
%type <fieldDecl> field_decl
%type <nameList> name_list
%type <varPart> var_part
%type <varDeclList> var_decl_list
%type <varDecl> var_decl
%type <routinePart> routine_part
%type <functionDecl> function_decl
%type <functionHead> function_head
%type <procedureDecl> procedure_decl
%type <procedureHead> procedure_head
%type <parameters> parameters
%type <paraDeclList> para_decl_list
%type <paraTypeList> para_type_list
%type <varParaList> var_para_list
%type <valParaList> val_para_list
%type <routineBody> routine_body
%type <compountStmt> compount_stmt
%type <stmtList> stmt_list
%type <stmt> stmt
%type <nonLabelStmt> non_label_stmt
%type <assignStmt> assign_stmt
%type <procStmt> proc_stmt
%type <ifStmt> if_stmt
%type <elseClause> else_clause
%type <repeatStmt> repeat_stmt
%type <whileStmt> while_stmt
%type <forStmt> for_stmt
%type <direction> direction
%type <caseStmt> case_stmt
%type <caseExprList> case_expr_list
%type <caseExpr> case_expr
%type <gotoStmt> goto_stmt
%type <expressionList> expression_list
%type <expression> expression
%type <expr> expr
%type <term> term
%type <factor> factor
%type <argsList> args_list

// %left PLUS MINUS
// %left MUL DIV

%start program

%%


program : program_head  routine  DOT
program_head : PROGRAM  ID  SEMI
routine : routine_head  routine_body
sub_routine : routine_head  routine_body

routine_head : label_part  const_part  type_part  var_part  routine_part
label_part : empty
const_part : CONST  const_expr_list  |  empty
const_expr_list : const_expr_list  NAME  EQUAL  const_value  SEMI
|  NAME  EQUAL  const_value  SEMI
const_value : INTEGER  |  REAL  |  CHAR  |  STRING  |  SYS_CON
type_part : TYPE type_decl_list  |  empty
type_decl_list : type_decl_list  type_definition  |  type_definition
type_definition : NAME  EQUAL  type_decl  SEMI
type_decl : simple_type_decl  |  array_type_decl  |  record_type_decl
simple_type_decl : SYS_TYPE  |  NAME  |  LP  name_list  RP  
                |  const_value  DOTDOT  const_value  
                |  MINUS  const_value  DOTDOT  const_value
                |  MINUS  const_value  DOTDOT  MINUS  const_value
                |  NAME  DOTDOT  NAME
array_type_decl : ARRAY  LB  simple_type_decl  RB  OF  type_decl
record_type_decl : RECORD  field_decl_list  END
field_decl_list : field_decl_list  field_decl  |  field_decl
field_decl : name_list  COLON  type_decl  SEMI
name_list : name_list  COMMA  ID  |  ID
var_part : VAR  var_decl_list  |  empty
var_decl_list :  var_decl_list  var_decl  |  var_decl
var_decl :  name_list  COLON  type_decl  SEMI

routine_part :  routine_part  function_decl  
                  |  routine_part  procedure_decl 
                  |  function_decl  
                  |  procedure_decl  
                  |  empty
function_decl : function_head  SEMI  sub_routine  SEMI
function_head :  FUNCTION  NAME  parameters  COLON  simple_type_decl 
procedure_decl :  procedure_head  SEMI  sub_routine  SEMI
procedure_head :  PROCEDURE NAME parameters 
parameters : LP  para_decl_list  RP  |  empty
para_decl_list : para_decl_list  SEMI  para_type_list | para_type_list
para_type_list : var_para_list COLON  simple_type_decl  
                  |  val_para_list  COLON  simple_type_decl
var_para_list : VAR  name_list
val_para_list : name_list

routine_body : compound_stmt
compound_stmt : BEGIN  stmt_list  END
stmt_list : stmt_list  stmt  SEMI  |  empty
stmt : INTEGER  COLON  non_label_stmt  
            | non_label_stmt
non_label_stmt : assign_stmt 
            | proc_stmt 
            | compound_stmt 
            | if_stmt 
            | repeat_stmt 
            | while_stmt 
            | for_stmt 
            | case_stmt 
            | goto_stmt
assign_stmt : ID  ASSIGN  expression
           | ID LB expression RB ASSIGN expression
           | ID  DOT  ID  ASSIGN  expression
proc_stmt : ID
          |  ID  LP  args_list  RP
          |  SYS_PROC
          |  SYS_PROC  LP  expression_list  RP
          |  READ  LP  factor  RP
if_stmt : IF  expression  THEN  stmt  else_clause
else_clause : ELSE stmt |  empty
repeat_stmt : REPEAT  stmt_list  UNTIL  expression
while_stmt : WHILE  expression  DO stmt
for_stmt : FOR  ID  ASSIGN  expression  direction  expression  DO stmt
direction : TO | DOWNTO
case_stmt : CASE expression OF case_expr_list  END
case_expr_list : case_expr_list  case_expr  
            |  case_expr
case_expr : const_value  COLON  stmt  SEMI
          |  ID  COLON  stmt  SEMI
goto_stmt : GOTO  INTEGER
expression_list : expression_list  COMMA  expression  
            |  expression
expression : expression  GE  expr  
            |  expression  GT  expr  
            |  expression  LE  expr
            |  expression  LT  expr 
            |  expression  EQ  expr  
            |  expression  NE  expr  
            |  expr
expr : expr  PLUS  term  
      |  expr  MINUS  term  
      |  expr  OR  term  
      |  term
term : term  MUL  factor  
      |  term  DIV  factor  
      |  term  MOD  factor 
      |  term  AND  factor    
      |  factor
factor : NAME  
      |  NAME  LP  args_list  RP  
      |  SYS_FUNCT 
      |  SYS_FUNCT  LP  args_list  RP  
      |  const_value  
      |  LP  expression  RP
      |  NOT  factor    
      |  MINUS  factor  
      |  ID  LB  expression  RB
      |  ID  DOT  ID
args_list : args_list  COMMA  expression  |  expression

empty :

%%