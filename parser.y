%{
    #include "ASTNode.h"
    Program *astRoot;

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

    std::string *string;
    int token;
}

%token <string> SYS_CON SYS_FUNCT SYS_PROC SYS_TYPE 
%token <string> ID INTEGER DOUBLE
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
%type <string> NAME

// %left PLUS MINUS
// %left MUL DIV

%start program

%%
program: program_head  routine  DOT {astRoot = new Program($1, $2);}
program_head: PROGRAM  ID  SEMI {$$ = new ProgramHead(*$2);}
routine: routine_head  routine_body {$$ = new Routine($1, $2);}
sub_routine: routine_head  routine_body {$$ = new SubRoutine($1, $2);}

routine_head: label_part  const_part  type_part  var_part  routine_part {$$ = new RoutineHead($1, $2, $3, $4, $5);}
label_part: empty {$$ = new LabelPart();}
const_part: CONST  const_expr_list  |  empty {$$ = new ConstPart($2);}
const_expr_list: const_expr_list  NAME  EQUAL  const_value  SEMI {$$ = new ConstExprList(*$2, $1, $4);}
                  |  NAME  EQUAL  const_value  SEMI {$$ = new ConstExprList(*$1, nullptr, $3);}
const_value: INTEGER  {$$ = new ConstValue(*$1, ConstValue::INTEGER);}
                  |  REAL  {$$ = new ConstValue(*$1, ConstValue::REAL);}
                  |  CHAR  {$$ = new ConstValue(*$1, ConstValue::CHAR);}
                  |  STRING  {$$ = new ConstValue(*$1, ConstValue::STRING);}
                  |  SYS_CON {$$ = new ConstValue(*$1, ConstValue::SYS_CON);}
type_part: TYPE type_decl_list {$$ = new TypePart($1);}
                  |  empty {$$ = new TypePart(nullptr);}
type_decl_list: type_decl_list  type_definition {$$ = new TypeDeclList($1, $2);}
                  |  type_definition {$$ = new TypeDeclList(nullptr, $1);}
type_definition: NAME  EQUAL  type_decl  SEMI {$$ = new TypeDefinition(*$1, $2);}
type_decl: simple_type_decl {$$ = new TypeDecl($1);}
                  |  array_type_decl {$$ = new TypeDecl($1);}
                  |  record_type_decl {$$ = new TypeDecl($1);}
simple_type_decl: SYS_TYPE {$$ = new SimpleTypeDecl(SimpleTypeDecl::SYS_TYPE, *$1);}
                  |  NAME  {$$ = new SimpleTypeDecl(SimpleTypeDecl::TYPE_NAME, *$1);}
                  |  LP  name_list  RP {$$ = new SimpleTypeDecl($2);}
                  |  const_value  DOTDOT  const_value  {$$ = new SimpleTypeDecl($1, $3);}
                  |  MINUS  const_value  DOTDOT  const_value {$$ = new SimpleTypeDecl($2->negate(), $4);}
                  |  MINUS  const_value  DOTDOT  MINUS  const_value {$$ = new SimpleTypeDecl($2->negate(), $5->negate());}
                  |  NAME  DOTDOT  NAME {$$ = new SimpleTypeDecl(*$1, *$3);}
array_type_decl: ARRAY  LB  simple_type_decl  RB  OF  type_decl {$$ = new ArrayTypeDecl($3, $6);}
record_type_decl: RECORD  field_decl_list  END {$$ = new RecordTypeDecl($2);}
field_decl_list: field_decl_list  field_decl {$$ = new FieldDeclList($1, $2);}
                  |  field_decl {$$ = new FieldDeclList(nullptr, $1);}
field_decl: name_list  COLON  type_decl  SEMI {$$ = new FieldDecl($1, $3);}
name_list: name_list  COMMA  ID {$$ = new NameList($1, *$3);}
                  |  ID {$$ = new NameList(nullptr, *$1);}
var_part: VAR  var_decl_list {$$ = new VarPart($2);}
                  |  empty {$$ = new VarPart(nullptr);}
var_decl_list:  var_decl_list  var_decl {$$ = new VarDeclList($1, $2);}
                  |  var_decl {$$ = new VarDeclList(nullptr, $1);}
var_decl:  name_list  COLON  type_decl  SEMI {$$ = new VarDecl($1, $3);}

routine_part:  routine_part  function_decl {$$ = new RoutinePart($1, $2);}
                  |  routine_part  procedure_decl {$$ = new RoutinePart($1, $2);}
                  |  function_decl  {$$ = new RoutinePart($1);}
                  |  procedure_decl  {$$ = new RoutinePart($1);}
                  |  empty {$$ = new RoutinePart(RoutinePart::EMPTY);}
function_decl: function_head  SEMI  sub_routine  SEMI {$$ = new FunctoinDecl($1, $3);}
function_head:  FUNCTION  NAME  parameters  COLON  simple_type_decl {$$ = new FunctionHead(*$2, $3, $5);}
procedure_decl:  procedure_head  SEMI  sub_routine  SEMI {$$ = new ProcedureDecl($1, $3);}
procedure_head:  PROCEDURE NAME parameters {$$ = new ProcedureHead(*$2, $3);}
parameters: LP  para_decl_list  RP {$$ = new Parameters($2);}
                  |  empty {$$ = new Parameters(nullptr);}
para_decl_list: para_decl_list  SEMI  para_type_list {$$ = new ParaDeclList($1, $2);}
                  | para_type_list {$$ = new ParaDeclList(nullptr, $1);}
para_type_list: var_para_list COLON  simple_type_decl {$$ = new ParaTypeList($1, $2);}
                  |  val_para_list  COLON  simple_type_decl {$$ = new ParaTypeList($1, $2);}
var_para_list: VAR  name_list {$$ = new VarParaList($2);}
val_para_list: name_list {$$ = new ValParaList($1);}

routine_body: compound_stmt {$$ = new RoutineBody($1);}
compound_stmt: BEGIN  stmt_list  END {$$ = new CompoundStmt($2);}
stmt_list: stmt_list  stmt  SEMI  {$$ = new StmtList($1, $2);}
                  |  empty {$$ = new StmtList(nullptr, nullptr);}
stmt: INTEGER  COLON  non_label_stmt {$$ = new Stmt(Stmt::LABELED, $3);}
            | non_label_stmt {$$ = new Stmt(Stmt::UNLABELED, $1);}
non_label_stmt: assign_stmt {$$ = new NonLabelStmt($1);}
            | proc_stmt {$$ = new NonLabelStmt($1);}
            | compound_stmt {$$ = new NonLabelStmt($1);}
            | if_stmt {$$ = new NonLabelStmt($1);}
            | repeat_stmt {$$ = new NonLabelStmt($1);}
            | while_stmt {$$ = new NonLabelStmt($1);}
            | for_stmt {$$ = new NonLabelStmt($1);}
            | case_stmt {$$ = new NonLabelStmt($1);}
            | goto_stmt {$$ = new NonLabelStmt($1);}
assign_stmt: ID  ASSIGN  expression {$$ = new AssignStmt(*$1, $3);}
           | ID LB expression RB ASSIGN expression {$$ = new AssignStmt(*$1, $3, $6);}
           | ID  DOT  ID  ASSIGN  expression {$$ = new AssignStmt(*$1, *$3, $5);}
proc_stmt: ID
          |  ID  LP  args_list  RP
          |  SYS_PROC
          |  SYS_PROC  LP  expression_list  RP
          |  READ  LP  factor  RP
if_stmt: IF  expression  THEN  stmt  else_clause
else_clause: ELSE stmt |  empty
repeat_stmt: REPEAT  stmt_list  UNTIL  expression
while_stmt: WHILE  expression  DO stmt
for_stmt: FOR  ID  ASSIGN  expression  direction  expression  DO stmt
direction: TO | DOWNTO
case_stmt: CASE expression OF case_expr_list  END
case_expr_list: case_expr_list  case_expr  
            |  case_expr
case_expr: const_value  COLON  stmt  SEMI
          |  ID  COLON  stmt  SEMI
goto_stmt: GOTO  INTEGER
expression_list: expression_list  COMMA  expression  
            |  expression
expression: expression  GE  expr  
            |  expression  GT  expr  
            |  expression  LE  expr
            |  expression  LT  expr 
            |  expression  EQ  expr  
            |  expression  NE  expr  
            |  expr
expr: expr  PLUS  term  
      |  expr  MINUS  term  
      |  expr  OR  term  
      |  term
term: term  MUL  factor  
      |  term  DIV  factor  
      |  term  MOD  factor 
      |  term  AND  factor    
      |  factor
factor: NAME  
      |  NAME  LP  args_list  RP  
      |  SYS_FUNCT 
      |  SYS_FUNCT  LP  args_list  RP  
      |  const_value  
      |  LP  expression  RP
      |  NOT  factor    
      |  MINUS  factor  
      |  ID  LB  expression  RB
      |  ID  DOT  ID
args_list: args_list  COMMA  expression  |  expression
NAME: ID {$$ = $1}
empty:

%%