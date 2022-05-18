%locations

%{
#include <cstdio>
#include <cstdarg>
#include <vector>
#include <string>
#include <iostream>
#include <chocopy_parse.hpp>
#include <chocopy_ast.hpp>


/** external functions from lex */
extern void yyrestart(FILE*);
extern int yylex();
extern int yyparse();
extern FILE* yyin;
parser::Program *ROOT = new parser::Program(new int[4]{1, 1, 1, 1});

typedef struct yyltype {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} yyltype;

/** Error reporting */
void yyerror(const char *s);


/** Return a mutable list initially containing the single value ITEM. */
template<typename T>
std::vector<T *>* single(T *item) {
    std::vector<T *> *list=new std::vector<T *>();
    list->push_back(item);
    return list;
}

/* If ITEM is non-null, appends it to the end of LIST.  Then returns LIST. Generic is deprecated */
template<typename T>
std::vector<parser::Decl *>* combine(std::vector<parser::Decl *>* list, T *item) {
    list->push_back(item);
    return list;
}


std::vector<parser::Stmt *>* combine(std::vector<parser::Stmt *>* list, parser::Stmt *item) {
    list->push_back(item);
    return list;
}

parser::ListExpr* combine(parser::ListExpr *list, parser::Expr *item) {
    list->elements->push_back(item);
    return list;
}

std::vector<parser::TypedVar *>* combine(std::vector<parser::TypedVar *>* list, parser::TypedVar *item) {
    list->push_back(item);
    return list;
}

std::vector<parser::IfStmt *>* combine(std::vector<parser::IfStmt *>* list, parser::IfStmt *item) {
    list->push_back(item);
    return list;
}

template<typename T>
T* get_right (std::vector<T*> *item){
    return item->at(0);
}
%}


%union {
  char * raw_str;
  int raw_int;
  const char *error_msg;
  ::parser::Program *PtrProgram;
  ::parser::Stmt *PtrStmt;
  ::parser::Decl *PtrDecl;
  ::parser::AssignStmt *PtrAssignStmt;
  ::parser::BinaryExpr *PtrBinaryExpr;
  ::parser::BoolLiteral *PtrBoolLiteral;
  ::parser::CallExpr *PtrCallExpr;
  ::parser::ClassDef *PtrClassDef;
  ::parser::ClassType *PtrClassType;
  ::parser::CompilerErr *PtrCompilerErr;
  ::parser::Err *PtrErr;
  ::parser::Expr *PtrExpr;
  ::parser::ExprStmt *PtrExprStmt;
  ::parser::ForStmt *PtrForStmt;
  ::parser::FuncDef *PtrFuncDef;
  ::parser::GlobalDecl *PtrGlobalDecl;
  ::parser::Ident *PtrIdent;
  ::parser::IfExpr *PtrIfExpr;
  ::parser::IndexExpr *PtrIndexExpr;
  ::parser::IntegerLiteral *PtrIntegerLiteral;
  ::parser::ListExpr *PtrListExpr;
  ::parser::ListType *PtrListType;
  ::parser::Literal *PtrLiteral;
  ::parser::MemberExpr *PtrMemberExpr;
  ::parser::MethodCallExpr *PtrMethodCallExpr;
  ::parser::Node *PtrNode;
  ::parser::NoneLiteral *PtrNoneLiteral;
  ::parser::NonlocalDecl *PtrNonlocalDecl;
  ::parser::ReturnStmt *PtrReturnStmt;
  ::parser::StringLiteral *PtrStringLiteral;
  ::parser::TypeAnnotation *PtrTypeAnnotation;
  ::parser::TypedVar *PtrTypedVar;
  ::parser::UnaryExpr *PtrUnaryExpr;
  ::parser::VarDef *PtrVarDef;
  ::parser::WhileStmt *PtrWhileStmt;
  ::parser::IfStmt *PtrIfStmt;
  ::parser::AccOptions *PtrAccOptions;
  std::vector<::parser::Decl *> *PtrListDecl;
  std::vector<::parser::Stmt *> *PtrListStmt;
  std::vector<::parser::TypedVar *> *PtrListTypedVar;
}


/* declare tokens */
%token <raw_int> TOKEN_INTEGER
%token <raw_str> TOKEN_IDENTIFIER
%token <raw_str> TOKEN_STRING
%token <raw_str> TOKEN_TRUE
%token <raw_str> TOKEN_FALSE
%token <raw_str> TOKEN_AND
%token <raw_str> TOKEN_BREAK
%token <raw_str> TOKEN_DEF
%token <raw_str> TOKEN_ELIF
%token <raw_str> TOKEN_ELSE
%token <raw_str> TOKEN_FOR
%token <raw_str> TOKEN_IF
%token <raw_str> TOKEN_NOT
%token <raw_str> TOKEN_OR
%token <raw_str> TOKEN_RETURN
%token <raw_str> TOKEN_WHILE
%token <raw_str> TOKEN_NONE
%token <raw_str> TOKEN_AS
%token <raw_str> TOKEN_CLASS
%token <raw_str> TOKEN_GLOBAL
%token <raw_str> TOKEN_IN
%token <raw_str> TOKEN_IS
%token <raw_str> TOKEN_NONLOCAL
%token <raw_str> TOKEN_PASS
/* For ACC support */
%token <raw_str> TOKEN_DECORATOR
%token <raw_str> TOKEN_ACC
%token <raw_str> TOKEN_PARALLEL
%token <raw_str> TOKEN_LOOP
%token <raw_str> TOKEN_VECTOR
%token <raw_str> TOKEN_WORKER

%token <raw_str> TOKEN_plus
%token <raw_str> TOKEN_minus
%token <raw_str> TOKEN_star
%token <raw_str> TOKEN_slash
%token <raw_str> TOKEN_percent
%token <raw_str> TOKEN_less
%token <raw_str> TOKEN_greater
%token <raw_str> TOKEN_lessequal
%token <raw_str> TOKEN_greaterequal
%token <raw_str> TOKEN_equalequal
%token <raw_str> TOKEN_exclaimequal
%token <raw_str> TOKEN_equal
%token <raw_str> TOKEN_l_paren
%token <raw_str> TOKEN_r_paren
%token <raw_str> TOKEN_l_square
%token <raw_str> TOKEN_r_square
%token <raw_str> TOKEN_comma
%token <raw_str> TOKEN_colon
%token <raw_str> TOKEN_period
%token <raw_str> TOKEN_rarrow
%token TOKEN_INDENT
%token TOKEN_DEDENT
%token TOKEN_NEWLINE

%type <PtrProgram> program
%type <PtrStmt> stmt expr_stmt
%type <PtrIfStmt> elif_list
%type <PtrListDecl> top_level_decl declaration_list class_helper
%type <PtrListStmt> stmt_list block
%type <PtrListTypedVar> typed_var_list
%type <PtrClassDef> class_def
%type <PtrGlobalDecl> global_decl
%type <PtrTypedVar> typed_var
%type <PtrTypeAnnotation> type
%type <PtrExpr> expr cexpr optionalRet assign_expr binary_expr
%type <PtrIndexExpr> index_expr
%type <PtrMemberExpr> member_expr
%type <PtrListExpr> list_expr
%type <PtrLiteral> literal
%type <PtrIdent> identifier
%type <PtrVarDef> var_def
%type <PtrFuncDef> func_def
%type <PtrNonlocalDecl> nonlocal_decl
%type <PtrAssignStmt> assign_stmt
%type <PtrAccOptions> acc_options


%left TOKEN_OR
%left TOKEN_AND
%left TOKEN_NOT
%nonassoc TOKEN_equalequal TOKEN_exclaimequal TOKEN_greater TOKEN_greaterequal TOKEN_less TOKEN_lessequal TOKEN_IS
%left TOKEN_plus TOKEN_minus
%left TOKEN_star TOKEN_slash TOKEN_percent
%right TOKEN_period TOKEN_comma TOKEN_l_square TOKEN_r_square
%right TOKEN_ELIF

%start program

%%
/* The grammar rules Your Code Here */

%%

/** The error reporting function. */
void yyerror(const char *s) {
    /** TO STUDENTS: This is just an example.
     * You can customize it as you like. */
    info = "Parser error near token ";
    info += yylval.error_msg;
    int *loc = new int[4]{0};
    loc[0] = yylloc.first_line;
    loc[1] = yylloc.first_column;
    loc[2] = yylloc.last_line;
    loc[3] = yylloc.last_column - 1;

    parser::CompilerErr *test = new parser::CompilerErr(loc, info, true);
    ROOT->errors->compiler_errors = new vector<parser::CompilerErr *>();
    ROOT->has_compiler_errors = true;
    ROOT->errors->compiler_errors->push_back(test);
    ((parser::Node *)ROOT)->location[2] = e1;
    ((parser::Node *)ROOT)->location[3] = e2;
}

parser::Program * parse(const char* input_path) {
    if (input_path != NULL) {
        if (!(yyin = fopen(input_path, "r"))) {
            fprintf(stderr, "[ERR] Open input file %s failed.\n", input_path);
            exit(EXIT_FAILURE);
        }
    } else {
        yyin = stdin;
    }
    /** Uncomment to see the middle process of bison*/
    // yydebug = 1;
    yyrestart(yyin);
    yyparse();
    return ROOT;
}
