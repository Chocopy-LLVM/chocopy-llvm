//
// Created by yiwei yang on 2021/7/26.
//

#ifndef CHOCOPY_COMPILER_CHOCOPY_SEMANT_HPP
#define CHOCOPY_COMPILER_CHOCOPY_SEMANT_HPP

#include "ClassDefType.hpp"
#include "FunctionDefType.hpp"
#include "SymbolTable.hpp"
#include "SymbolType.hpp"
#include "ValueType.hpp"
#include <chocopy_logging.hpp>
#include <map>
#include <stack>
#include <string>

using std::stack;

namespace parser {
class AssignStmt;
class BinaryExpr;
class BoolLiteral;
class CallExpr;
class ClassDef;
class ClassType;
class Decl;
class CompilerErr;
class Expr;
class ExprStmt;
class ForStmt;
class FuncDef;
class GlobalDecl;
class Ident;
class IfExpr;
class IndexExpr;
class IntegerLiteral;
class ListExpr;
class ListType;
class Literal;
class MemberExpr;
class MethodCallExpr;
class Node;
class NoneLiteral;
class NonlocalDecl;
class Program;
class ReturnStmt;
class Stmt;
class StringLiteral;
class TypeAnnotation;
class TypedVar;
class UnaryExpr;
class VarDef;
class PassStmt;
class IfStmt;
class Err;
class WhileStmt;
class VarAssignStmt;
class MemberAssignStmt;
class IndexAssignStmt;
class VarAssignExpr;
class MemberAssignExpr;
class IndexAssignExpr;

} // namespace parser

using std::map;
using std::string;

namespace ast {
class Visitor;
class ASTAnalyzer : public Visitor {
public:
    virtual void visit(parser::BinaryExpr &node){};
    virtual void visit(parser::Node &node){};
    virtual void visit(parser::Err &node){};
    virtual void visit(parser::PassStmt &node){};
    virtual void visit(parser::BoolLiteral &node){};
    virtual void visit(parser::CallExpr &node){};
    virtual void visit(parser::ClassDef &node){};
    virtual void visit(parser::ClassType &node){};
    virtual void visit(parser::ExprStmt &node){};
    virtual void visit(parser::ForStmt &node){};
    virtual void visit(parser::FuncDef &node){};
    virtual void visit(parser::GlobalDecl &node){};
    virtual void visit(parser::Ident &node){};
    virtual void visit(parser::IfExpr &node){};
    virtual void visit(parser::IfStmt &node){};
    virtual void visit(parser::IndexExpr &node){};
    virtual void visit(parser::IntegerLiteral &node){};
    virtual void visit(parser::ListExpr &node){};
    virtual void visit(parser::ListType &node){};
    virtual void visit(parser::MemberExpr &node){};
    virtual void visit(parser::MethodCallExpr &node){};
    virtual void visit(parser::NoneLiteral &node){};
    virtual void visit(parser::NonlocalDecl &node){};
    virtual void visit(parser::ReturnStmt &node){};
    virtual void visit(parser::StringLiteral &node){};
    virtual void visit(parser::TypedVar &node){};
    virtual void visit(parser::UnaryExpr &node){};
    virtual void visit(parser::VarDef &node){};
    virtual void visit(parser::WhileStmt &node){};
    virtual void visit(parser::VarAssignStmt &){};
    virtual void visit(parser::MemberAssignStmt &){};
    virtual void visit(parser::IndexAssignStmt &){};
    virtual void visit(parser::VarAssignExpr &){};
    virtual void visit(parser::MemberAssignExpr &){};
    virtual void visit(parser::IndexAssignExpr &){};
    virtual void visit(parser::TypeAnnotation &){};
    virtual void visit(parser::AssignStmt &node){};
    virtual void visit(parser::Program &node){};
};

} // namespace ast

namespace semantic {

class SemanticError : public parser::Err {
public:
    SemanticError(Node *node, const string &message) : Err(nullptr, false), message(message) {
        if (node->location != nullptr) {
            this->location = node->get_location();
        }
        ((parser::Node *)this)->error_msg = message;
        ((parser::Node *)this)->kind = "SemanticError";
    }

    cJSON *toJSON() override;
    string message;
};

/** Analyzer that performs ChocoPy type checks on all nodes.  Applied after
 *  collecting declarations. */
class TypeChecker : public ast::ASTAnalyzer {
public:
    void visit(parser::BinaryExpr &node) override;
    void visit(parser::BoolLiteral &node) override;
    void visit(parser::CallExpr &node) override;
    void visit(parser::ClassDef &node) override;
    void visit(parser::ExprStmt &node) override;
    void visit(parser::ForStmt &node) override;
    void visit(parser::FuncDef &node) override;
    void visit(parser::Ident &node) override;
    void visit(parser::IfStmt &node) override;
    void visit(parser::IfExpr &node) override;
    void visit(parser::IndexExpr &node) override;
    void visit(parser::IntegerLiteral &node) override;
    void visit(parser::ListExpr &node) override;
    void visit(parser::MemberExpr &node) override;
    void visit(parser::MethodCallExpr &node) override;
    void visit(parser::NoneLiteral &node) override;
    void visit(parser::ReturnStmt &node) override;
    void visit(parser::StringLiteral &node) override;
    void visit(parser::UnaryExpr &node) override;
    void visit(parser::VarDef &node) override;
    void visit(parser::WhileStmt &node) override;
    void visit(parser::AssignStmt &) override;
    void visit(parser::VarAssignStmt &) override;
    void visit(parser::MemberAssignStmt &) override;
    void visit(parser::IndexAssignStmt &) override;
    void visit(parser::VarAssignExpr &) override;
    void visit(parser::IndexAssignExpr &) override;
    void visit(parser::TypedVar &node) override{};
    void visit(parser::Program &node) override;

    /** type checker attributes and their chocopy typing judgement analogues:
     * O : symbolTable
     * M : classes
     * C : currentClass
     * R : expReturnType */
    TypeChecker(SymbolTable *globalSymbols, vector<parser::Err *> *errors0) {
        this->sym = globalSymbols;
        this->globals = globalSymbols;
        this->errors = errors0;
        setup_num_to_class();
        debug_sym();
    }
    /** Inserts an error message in NODE if there isn"t one already.
     *  The message is constructed with MESSAGE and ARGS as for
     *  String.format. */
    template <typename... Args> void typeError(parser::Node *node, const string &message, Args... rest);
    static void typeError(parser::Node *node, const string &message);

    /** The current symbol table (changes depending on the function
     *  being analyzed). */
    SymbolTable *sym;
    stack<SymbolTable *> saved{};
    /** always global */
    SymbolTable *globals{};

    /** For the nested function declaration */
    FunctionDefType *curr_func = nullptr;
    stack<FunctionDefType *> saved_func{};

    SymbolType *passing_type{};

    /** Collector for errors. */
    vector<parser::Err *> *errors;

    /** set up default class hierarchy
     * <None> <= object
     * <Empty> <= object
     * <None> <= <None>
     * <Empty> <= <Empty>
     */
    map<string, string> super_classes = {{"int", "object"},   {"bool", "int"},      {"none", "object"},
                                         {"empty", "object"}, {"<None>", "object"}, {"<Empty>", "object"}};

    string get_common_type(SymbolType *first, SymbolType *second);
    bool is_sub_type(SymbolType *first, SymbolType *second);
    void setup_num_to_class();

    /** linear-list-stored graph for the object graph
     *
     * OBJECT 0
     * INT1 BOOL2 STR3 LIST4 A5   F10 ....
     *                       | \
     *                      B6 E9
     *                      | \
     *                      C7 D8
     */

    /** initialize the edge */
    void add_edge(const string &a, const string &b) {
        if (!sym->head.count(a))
            sym->head[a] = -1;
        if (!sym->head.count(b))
            sym->head[b] = -1;
        sym->way.emplace_back(SymbolTable::edge{b, sym->head[a]});
        sym->head[a] = sym->way.size() - 1;
    }
    int dfn = -1;
    /** depth first search */
    void dfs(const string &x) {
        sym->lhs[x] = ++dfn;
        sym->nametable.emplace_back(x);
        for (int y = sym->head[x]; y != -1; y = sym->way[y].pre) {
            dfs(sym->way[y].target);
        }
        sym->rhs[x] = dfn;
        if (x != "object")
            sym->class_tag_[x] = sym->lhs[x] + 3;
        else
            sym->class_tag_[x] = sym->lhs[x];
    }

    void debug_sym();
    void debug_nested_func_sym(SymbolTable *func_sym, int tab);
};

/**
 * Analyzes declarations to create a top-level symbol table
 */
class DeclarationAnalyzer : public ast::ASTAnalyzer {
public:
    void visit(parser::ClassDef &node) override;
    void visit(parser::ClassType &node) override;
    void visit(parser::IfStmt &node) override;
    void visit(parser::FuncDef &node) override;
    void visit(parser::GlobalDecl &node) override;
    void visit(parser::ListType &node) override;
    void visit(parser::NonlocalDecl &node) override;
    void visit(parser::TypedVar &node) override;
    void visit(parser::VarDef &varDef) override;
    void visit(parser::Program &program) override;
    void visit(parser::ForStmt &s) override;

    explicit DeclarationAnalyzer(vector<parser::Err *> *errors);

    SymbolTable *getGlobals() { return sym; }

    /** Collector for errors. */
    vector<parser::Err *> *errors;

private:
    /** Changes with scope */
    SymbolTable *sym = new SymbolTable;

    /** always global */
    SymbolTable *globals = sym;

    string curr_class;
    string curr_func;
    SymbolType *passing_type = nullptr;

    void debug_sym();
    vector<string> shadow_class_table;
    vector<string> shadow_var_table;
    bool check_shadow_again(const string &symbol);
    bool check_var_again(const string &symbol);
};

} // namespace semantic
#endif // CHOCOPY_COMPILER_CHOCOPY_SEMANT_HPP
