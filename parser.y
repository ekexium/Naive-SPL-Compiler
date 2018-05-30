%{
    #include "ASTNode.h"
    NBlock *programBlock; /* the top level root node of our final AST */

    extern int yylex();
    void yyerror(const char *s) { printf("ERROR: %s/n", s); }
%}

/* Represents the many different ways we can access our data */
%union {
    Node *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::vector *varvec;
    std::vector *exprvec;
    std::string *string;
    int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token  TIDENTIFIER TINTEGER TDOUBLE
%token  TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token  TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT
%token  TPLUS TMINUS TMUL TDIV

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type  ident
%type  numeric expr
%type  func_decl_args
%type  call_args
%type  program stmts block
%type  stmt var_decl func_decl
%type  comparison

/* Operator precedence for mathematical operators */
%left PLUS MINUS
%left MUL DIV

%start program

%%

empty :
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

routine_part :  routine_part  function_decl  |  routine_part  procedure_decl 
|  function_decl  |  procedure_decl  | empty
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
stmt : INTEGER  COLON  non_label_stmt  |  non_label_stmt
non_label_stmt : assign_stmt | proc_stmt | compound_stmt | if_stmt | repeat_stmt | while_stmt 
| for_stmt | case_stmt | goto_stmt
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
case_expr_list : case_expr_list  case_expr  |  case_expr
case_expr : const_value  COLON  stmt  SEMI
          |  ID  COLON  stmt  SEMI
goto_stmt : GOTO  INTEGER
expression_list : expression_list  COMMA  expression  |  expression
expression : expression  GE  expr  |  expression  GT  expr  |  expression  LE  expr
          |  expression  LT  expr  |  expression  EQUAL  expr  
|  expression  UNEQUAL  expr  |  expr
expr : expr  PLUS  term  |  expr  MINUS  term  |  expr  OR  term  |  term
term : term  MUL  factor  |  term  DIV  factor  |  term  MOD  factor 
 |  term  AND  factor  |  factor
factor : NAME  |  NAME  LP  args_list  RP  |  SYS_FUNCT |
SYS_FUNCT  LP  args_list  RP  |  const_value  |  LP  expression  RP
|  NOT  factor  |  MINUS  factor  |  ID  LB  expression  RB
|  ID  DOT  ID
args_list : args_list  COMMA  expression  |  expression

%%