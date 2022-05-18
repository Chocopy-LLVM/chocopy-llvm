//
// Created by yiwei yang on 2/15/21.
//
#ifndef CHOCOPY_COMPILER_PARSE_HPP
#define CHOCOPY_COMPILER_PARSE_HPP

#pragma once

#include "FunctionDefType.hpp"
#include "ValueType.hpp"
#include <chocopy_ast.hpp>
#include <chocopy_logging.hpp>
#include <cjson/cJSON.h>
#include <iostream>
#include <utility>
#include <vector>

extern int e1, e2;
extern string info;

using namespace std;

namespace semantic {
class SymbolType;
class ClassValueType;
class ValueType;
} // namespace semantic

namespace ast {
class Visitor;
} // namespace ast

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
class WhileStmt;
class PassStmt;

class VarAssignStmt;
class MemberAssignStmt;
class IndexAssignStmt;
class VarAssignExpr;
class MemberAssignExpr;
class IndexAssignExpr;

class AccOptions;
/**
 * Root of the AST class hierarchy.  Every node has a left and right
 * location, indicating the start and end of the represented construct
 * in the source text.
 *
 * Every node can be marked with an error message, which serves two purposes:
 *   1. It indicates that an error message has been issued for this
 *      Node, allowing tne program to reduce cascades of error
 *      messages.
 *   2. It aids in debugging by making it convenient to see which
 *      Nodes have caused an error.
 */
class Node {
public:
    string kind;
    string error_msg;
    string typeError;

    int *location;
    /** Return my source location as
     *     { <first line>, <first column>, <last line>, <last column> }.
     *  Result should not be modified, and contents will change after
     *  setLocation(). */
    virtual int *get_location() { return nullptr; };

    /** Multiple Constructor. */
    explicit Node(int *location) {
        this->location = location;
        this->kind.clear();
        this->error_msg.clear();
    }
    Node(int *location, string kind) {
        this->location = location;
        this->kind = std::move(kind);
    }
    Node(int *location, string kind, string errorMsg) {
        this->location = location;
        this->kind = std::move(kind);
        this->error_msg = std::move(errorMsg);
    }

    virtual bool has_err() { return !this->error_msg.empty(); }
    virtual bool has_type_err() { return !this->typeError.empty(); }
    virtual cJSON *toJSON();
    virtual void accept(ast::Visitor &visitor);

    /** Add a new semantic error message attributed to NODE, with message
     *  String.format(MESSAGEFORM, ARGS). */
    template <typename T> void semError(T rest) { this->typeError += fmt::format(" {}", rest); }
    template <typename T, typename... Args> void semError(T message, Args... rest) {
        this->typeError += fmt::format(" {}", message);
        semError(rest...);
    }
};

/**
 * Base of all AST nodes representing definitions or declarations.
 */
class Decl : public Node {
public:
    /** A definition or declaration spanning source locations [LEFT..RIGHT]. */
    explicit Decl(int *location) : Node(location) {}
    Decl(int *location, string kind) : Node(location, std::move(kind)) {}
    Decl(int *location, string kind, string errMsg) : Node(location, std::move(kind), std::move(errMsg)) {}

    virtual Ident *get_id() { return nullptr; };
};

/** Represents a single error.  Does not correspond to any Python source
 *  construct. */
class CompilerErr : public Node {
public:
    string message;
    bool syntax = false;

    /** Represents an error with message MESSAGE.  Iff SYNTAX, it is a
     *  syntactic error.  The error applies to source text at [LEFT..RIGHT]. */
    CompilerErr(int *location, string message, bool syntax) : Node(location, "CompilerError") {
        this->message = std::move(message);
        this->syntax = syntax;
    };

    int *get_location() override { return this->location; }

    CompilerErr(int *location, string message) : Node(location, "CompilerError") { this->message = std::move(message); }
    cJSON *toJSON() override;
};

/** Collects the error messages in a Program.  There is exactly one per
 *  Program node. */
class Err : public Node {
public:
    /** The accumulated error messages in the order added. */
    vector<Err *> *errors;
    vector<CompilerErr *> *compiler_errors{};
    bool allow_multi_errors = true;
    bool top_level = true;

    /** An Errors whose list of CompilerErrors is ERRORS.  The list should be
     *  modified using this.Add. */
    Err(int *location, vector<Err *> *errors) : Node(location, "Errors") {
        ((Node *)this)->error_msg = "Error";
        this->errors = errors;
    }
    explicit Err(int *location) : Node(location, "Errors") { this->errors = new vector<Err *>(); }

    Err(int *location, bool top) : Node(location, "Errors"), top_level(top) { this->errors = new vector<Err *>(); }

    bool has_compiler_errors() const { return compiler_errors != nullptr; }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** An identifier with attached type annotation. */
class TypedVar : public Node {
public:
    Ident *identifier;
    TypeAnnotation *type;

    /** The AST for
     *       IDENTIFIER : TYPE.
     *  spanning source locations [LEFT..RIGHT].
     */
    TypedVar(int *location, Ident *identifier, TypeAnnotation *type) : Node(location, "TypedVar") {
        this->identifier = identifier;
        this->type = type;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
    semantic::SymbolType *inferredType{};
};

class Stmt : public Node {
public:
    bool is_return = false;
    explicit Stmt(int *location) : Node(location) {}
    Stmt(int *location, string kind) : Node(location, std::move(kind)) {}
    Stmt(int *location, string kind, string errMsg) : Node(location, std::move(kind), std::move(errMsg)) {}
};

/**
 * Base of all AST nodes representing expressions.
 *
 * There is nothing in this class, but there will be many AST
 * node types that have fields that are *any expression*. For those
 * cases, having a field of this type will encompass all types of
 * expressions such as binary expressions and literals that subclass
 * this class.
 */
class Expr : public Node {
public:
    /** A Python expression spanning source locations [LEFT..RIGHT]. */
    explicit Expr(int *location);
    Expr(int *location, string kind);
    Expr(int *location, string kind, string errMsg);

    virtual bool emit_inferred();

    int *get_location() override { return nullptr; };

    cJSON *toJSON() override;
    /**
     * The type of the value that this expression evaluates to.
     *
     * This field is always <tt>null</tt> after the parsing stage,
     * but is populated by the type checker in the semantic analysis
     * stage.
     *
     * After typechecking this field may be <tt>null</tt> only for
     * expressions that cannot be assigned a type. In particular,
     * {@link NoneLiteral} expressions will not have a typed assigned
     * to them.
     */
    semantic::SymbolType *inferredType{};
};

/** A simple identifier. */
class Ident : public Expr {
public:
    /** An AST for the variable, method, or parameter named NAME, spanning
     *  source locations [LEFT..RIGHT]. */
    Ident(int *location, string name) : Expr(location, "Identifier") { this->name = std::move(name); }

    int *get_location() override { return this->location; };

    string name;

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/**
 * Base of all the literal nodes.
 *
 * There is nothing in this class, but it is useful to isolate
 * expressions that are constant literals.
 */
class Literal : public Expr {
public:
    /** A literal spanning source locations [LEFT..RIGHT]. */
    explicit Literal(int *location) : Expr(location){};
    Literal(int *location, string kind) : Expr(location, std::move(kind)){};
    Literal(int *location, string kind, int value) : Expr(location, std::move(kind)), int_value(value), is_init(true){};
    Literal(int *location, string kind, string value)
        : Expr(location, std::move(kind)), value(std::move(value)), is_init(true){};

    cJSON *toJSON() override;

    string value;
    int int_value{};
    bool is_init = false;
};

/**
 * Base of all AST nodes representing type annotations (list or class
 * types.
 */
class TypeAnnotation : public Node {
public:
    /** An annotation spanning source locations [LEFT..RIGHT]. */
    explicit TypeAnnotation(int *location) : Node(location){};
    TypeAnnotation(int *location, string kind) : Node(location, std::move(kind)){};
    TypeAnnotation(int *location, string kind, string errMsg) : Node(location, std::move(kind), std::move(errMsg)){};

    string get_name();
};

/** Single and multiple assignments. DEPRECATED from semantic */
class AssignStmt : public Stmt {
public:
    vector<Expr *> *targets;
    Expr *value;

    /** AST for TARGETS[0] = TARGETS[1] = ... = VALUE spanning source locations
     *  [LEFT..RIGHT].
     */
    AssignStmt(int *location, vector<Expr *> *target, Expr *value) : Stmt(location, "AssignStmt") {
        this->targets = target;
        this->value = value;
    }

    AssignStmt(int *location, vector<Expr *> *target, AssignStmt *assign_stmt) : Stmt(location, "AssignStmt") {
        this->targets = target;
        for (auto &&item : *assign_stmt->targets) {
            this->targets->emplace_back(item);
        }
        this->value = assign_stmt->value;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** <operand> <operator> <operand>. */
class BinaryExpr : public Expr {
public:
    enum class operator_code {
        Plus = 0,
        Minus,
        Star,
        Slash,
        Percent,
        EqEq,
        NotEq,
        LessThan,
        MoreThan,
        More,
        Less,
        And,
        Or,
        Is,
        Unknown
    };

    Expr *left;
    string operator_;
    Expr *right;

    /** An AST for expressions of the form LEFTEXPR OP RIGHTEXPR
     *  from text in range [LEFTLOC..RIGHTLOC]. */
    BinaryExpr(int *location, Expr *leftExpr, string op, Expr *rightExpr) : Expr(location, "BinaryExpr") {
        this->left = leftExpr;
        this->right = rightExpr;
        this->operator_ = std::move(op);
    }

    static operator_code hashcode(std::string const &str) {
        if (str == "+")
            return operator_code::Plus;
        if (str == "-")
            return operator_code::Minus;
        if (str == "*")
            return operator_code::Star;
        if (str == "//")
            return operator_code::Slash;
        if (str == "%")
            return operator_code::Percent;
        if (str == "==")
            return operator_code::EqEq;
        if (str == "!=")
            return operator_code::NotEq;
        if (str == "<=")
            return operator_code::LessThan;
        if (str == ">=")
            return operator_code::MoreThan;
        if (str == ">")
            return operator_code::More;
        if (str == "<")
            return operator_code::Less;
        if (str == "and")
            return operator_code::And;
        if (str == "or")
            return operator_code::Or;
        if (str == "is")
            return operator_code::Is;
        return operator_code::Unknown;
    }
    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** Literals True or False. */
class BoolLiteral : public Literal {
public:
    /** True iff I represent True. */
    bool bin_value;

    /** An AST for the token True or False at [LEFT..RIGHT], depending on
     *  VALUE. */
    BoolLiteral(int *location, bool value) : Literal(location, "BooleanLiteral") { this->bin_value = value; }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** A function call. */
class CallExpr : public Expr {
public:
    Ident *function;
    vector<Expr *> *args{};
    bool has_args;

    CallExpr(int *location, Ident *function, vector<Expr *> *args) : Expr(location, "CallExpr") {
        this->function = function;
        this->args = args;
        this->has_args = true;
    }

    CallExpr(int *location, Ident *function) : Expr(location, "CallExpr") {
        this->function = function;
        this->has_args = false;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** A class definition. */
class ClassDef : public Decl {
public:
    Ident *name;
    Ident *superClass;
    vector<Decl *> *declaration;

    /** A class definition. */
    ClassDef(int *location, Ident *name, Ident *superClass, vector<Decl *> *declaration) : Decl(location, "ClassDef") {
        this->name = name;
        this->superClass = superClass;
        this->declaration = declaration;
    }

    int *get_location() override { return this->location; }
    Ident *get_id() override { return this->name; }
    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** A simple class type name. */
class ClassType : public TypeAnnotation {
public:
    string className;
    /** An AST denoting a type named CLASSNAME0 at [LEFT..RIGHT]. */
    ClassType(int *location, string className) : TypeAnnotation(location, "ClassType") {
        this->className = std::move(className);
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** A statement containing only an expression. */
class ExprStmt : public Stmt {
public:
    Expr *expr;
    ExprStmt(int *location, Expr *expr) : Stmt(location, "ExprStmt") { this->expr = expr; }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** For statements. */
class ForStmt : public Stmt {
public:
    Ident *identifier;
    Expr *iterable;
    vector<Stmt *> *body;
    AccOptions *accOptions = nullptr;

    /** The AST for
     *      for IDENTIFIER in ITERABLE:
     *          BODY
     *  spanning source locations [LEFT..RIGHT].
     */
    ForStmt(int *location, Ident *identifier, Expr *iterable, vector<Stmt *> *body) : Stmt(location, "ForStmt") {
        this->identifier = identifier;
        this->iterable = iterable;
        this->body = body;
    }

    /** The AST for
     *      for IDENTIFIER in ITERABLE:
     *          BODY
     *  spanning source locations [LEFT..RIGHT].
     */
    ForStmt(int *location, AccOptions *accOptions, Ident *identifier, Expr *iterable, vector<Stmt *> *body)
        : ForStmt(location, identifier, iterable, body) {
        this->accOptions = accOptions;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** Def statements. */
class FuncDef : public Decl {
public:
    Ident *name;
    Ident *decorator = nullptr;
    vector<TypedVar *> *params;
    TypeAnnotation *returnType = nullptr;
    vector<Decl *> *declarations;
    vector<Stmt *> *statements;
    vector<string> *lambda_params = new vector<string>();

    /** The AST for
     *     def NAME(PARAMS) -> RETURNTYPE:
     *         DECLARATIONS
     *         STATEMENTS
     *  spanning source locations [LEFT..RIGHT].
     */
    FuncDef(int *location, Ident *name, vector<TypedVar *> *params, TypeAnnotation *returnType,
            vector<Decl *> *declarations, vector<Stmt *> *statements)
        : Decl(location, "FuncDef") {
        this->name = name;
        this->params = params;
        this->returnType = returnType;
        this->declarations = declarations;
        this->statements = statements;
    }
    /** The AST for
     *     def NAME(PARAMS):
     *         DECLARATIONS
     *         STATEMENTS
     *  spanning source locations [LEFT..RIGHT].
     */
    FuncDef(int *location, Ident *name, vector<TypedVar *> *params, vector<Decl *> *declarations,
            vector<Stmt *> *statements)
        : Decl(location, "FuncDef") {
        this->name = name;
        this->params = params;
        this->declarations = declarations;
        this->statements = statements;
    }
    /** The AST for
     *     @decorator
     *     def NAME(PARAMS):
     *         DECLARATIONS
     *         STATEMENTS
     *  spanning source locations [LEFT..RIGHT].
     */
    FuncDef(int *location, Ident *decorator, Ident *name, vector<TypedVar *> *params, TypeAnnotation *returnType,
            vector<Decl *> *declarations, vector<Stmt *> *statements)
        : FuncDef(location, name, params, returnType, declarations, statements) {
        this->decorator = decorator;
    }
    int *get_location() override { return this->location; }
    Ident *get_id() override { return this->name; }
    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

class GlobalDecl : public Decl {
public:
    Ident *variable;
    GlobalDecl(int *location, Ident *variable) : Decl(location, "GlobalDecl") { this->variable = variable; }
    int *get_location() override { return this->location; }
    cJSON *toJSON() override;

    Ident *get_id() override { return this->variable; }
    void accept(ast::Visitor &visitor) override;
};

/** Conditional statement. */
class IfStmt : public Stmt {
public:
    enum cond { THEN_ELSE = 0, THEN_ELIF, THEN };
    /** Test condition. */
    Expr *condition{};
    /** "True" branch. */
    vector<Stmt *> *thenBody{};
    /** "False" branch. */
    vector<Stmt *> *elseBody{};
    IfStmt *elifBody{};
    /** Bool manifest else or elif or int */
    char el = cond::THEN;
    /** The AST for
     *      if CONDITION:
     *          THENBODY
     *      else:
     *          ELSEBODY
     *  spanning source locations [LEFT..RIGHT].
     */
    IfStmt(int *location, Expr *condition, vector<Stmt *> *thenBody, vector<Stmt *> *elseBody)
        : Stmt(location, "IfStmt") {
        this->condition = condition;
        this->thenBody = thenBody;
        this->elseBody = elseBody;
        this->el = cond::THEN_ELSE;
    }

    /** elseBody can be IfStmt to support polymorphism */
    IfStmt(int *location, Expr *condition, vector<Stmt *> *thenBody, IfStmt *elifBody) : Stmt(location, "IfStmt") {
        this->condition = condition;
        this->thenBody = thenBody;
        this->elifBody = elifBody;
        this->el = cond::THEN_ELIF;
    }

    IfStmt(int *location, Expr *condition, vector<Stmt *> *thenBody) : Stmt(location, "IfStmt") {
        this->condition = condition;
        this->thenBody = thenBody;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** Conditional expressions. */
class IfExpr : public Expr {
public:
    /** Boolean condition. */
    Expr *condition;
    /** True branch. */
    Expr *thenExpr;
    /** False branch. */
    Expr *elseExpr;

    /** The AST for
     *     THENEXPR if CONDITION else ELSEEXPR
     *  spanning source locations [LEFT..RIGHT].
     */
    IfExpr(int *location, Expr *condition, Expr *thenExpr, Expr *elseExpr) : Expr(location, "IfExpr") {
        this->condition = condition;
        this->thenExpr = thenExpr;
        this->elseExpr = elseExpr;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** List-indexing expression. */
class IndexExpr : public Expr {
public:
    /** Indexed list. */
    Expr *list;
    /** Expression for index value. */
    Expr *index;

    /** The AST for
     *      LIST[INDEX].
     *  spanning source locations [LEFT..RIGHT].
     */
    IndexExpr(int *location, Expr *list, Expr *index) : Expr(location, "IndexExpr") {
        this->list = list;
        this->index = index;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** Integer numerals. */
class IntegerLiteral : public Literal {
public:
    /** Value denoted. */
    int value;

    /** The AST for the literal VALUE, spanning source
     *  locations [LEFT..RIGHT]. */
    IntegerLiteral(int *location, int value) : Literal(location, "IntegerLiteral", value) { this->value = value; }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** List displays. */
class ListExpr : public Expr {
public:
    /** List of element expressions. */
    vector<Expr *> *elements{};
    bool has_expr;

    /** The AST for
     *      [ ELEMENTS ].
     *  spanning source locations [LEFT..RIGHT].
     */
    ListExpr(int *location, vector<Expr *> *elements) : Expr(location, "ListExpr") {
        this->elements = elements;
        this->has_expr = true;
    }
    explicit ListExpr(int *location) : Expr(location, "ListExpr") { this->has_expr = false; }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** Type denotation for a list type. */
class ListType : public TypeAnnotation {
public:
    TypeAnnotation *elementType;
    /** The AST for the type annotation
     *       [ ELEMENTTYPE ].
     *  spanning source locations [LEFT..RIGHT].
     */
    ListType(int *location, TypeAnnotation *element) : TypeAnnotation(location, "ListType") {
        this->elementType = element;
    }

    int *get_location() override { return this->location; }
    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** Attribute accessor. */
class MemberExpr : public Expr {
public:
    Expr *object;
    Ident *member;
    /** The AST for
     *     OBJECT.MEMBER.
     *  spanning source locations [LEFT..RIGHT].
     */
    MemberExpr(int *location, Expr *object, Ident *member) : Expr(location, "MemberExpr") {
        this->object = object;
        this->member = member;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;

    Ident *get_id() { return (Ident *)object; }
};

/** Method calls. */
class MethodCallExpr : public Expr {
public:
    /** Expression for the bound method to be called. */
    MemberExpr *method;
    /** Actual parameters. */
    vector<Expr *> *args{};
    bool has_args;
    /** The AST for
     *      METHOD(ARGS).
     *  spanning source locations [LEFT..RIGHT].
     */
    MethodCallExpr(int *location, MemberExpr *method, vector<Expr *> *args) : Expr(location, "MethodCallExpr") {
        this->location = location;
        this->args = args;
        this->method = method;
        this->has_args = true;
    }
    /** The initializer for unintialised args*/
    MethodCallExpr(int *location, MemberExpr *method) : Expr(location, "MethodCallExpr") {
        this->location = location;
        this->method = method;
        this->has_args = false;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** The expression 'None'. */
class NoneLiteral : public Literal {
public:
    /** The AST for None, spanning source locations [LEFT..RIGHT]. */
    explicit NoneLiteral(int *location) : Literal(location, "NoneLiteral") {}

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** Nonlocal declaration. */
class NonlocalDecl : public Decl {
public:
    Ident *variable;
    /** The AST for
     *      nonlocal VARIABLE
     *  spanning source locations [LEFT..RIGHT].
     */
    NonlocalDecl(int *location, Ident *variable) : Decl(location, "NonLocalDecl") { this->variable = variable; }

    int *get_location() override { return this->location; }
    Ident *get_id() override { return this->variable; }
    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

class Program : public Node {
public:
    vector<Stmt *> *statements = new vector<Stmt *>();
    Err *errors{};
    bool has_compiler_errors = false;
    bool has_semantic_errors = false;
    vector<Decl *> *declarations = new vector<Decl *>();
    int *location;

    Program(int *location, vector<Decl *> *declarations, vector<Stmt *> *statements) : Node(location, "Program") {
        /* self.declarations = [d for d in declarations if d is not None] */
        // printf("sv");
        if (!declarations->empty()) {
            this->declarations = declarations;
        } else {
            this->declarations = new vector<Decl *>();
        }
        /* self.statements = [s for s in statements if s is not None] */
        if (!statements->empty()) {
            this->statements = statements;
        } else {
            this->statements = new vector<Stmt *>();
        }
        /* Fix */
        int *i = new int[4]{0};
        this->location = location;
        this->errors = new Err(i);
    };

    explicit Program(int *location) : Node(location, "Program") {
        int *i = new int[4]{0};
        this->location = location;
        this->errors = new Err(i);
    };

    int *get_location() override { return this->location; }
    void add_error(Err *err) {
        this->errors = err;
        this->has_semantic_errors = true;
    }
    void add_error(vector<Err *> *err) {
        this->errors->errors = err;
        this->has_semantic_errors = true;
    }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** Return from function. */
class ReturnStmt : public Stmt {
public:
    /** Returned value. */
    Expr *value;
    /** The AST for
     *     return VALUE
     *  spanning source locations [LEFT..RIGHT].
     */
    ReturnStmt(int *location, Expr *value) : Stmt(location, "ReturnStmt") {
        this->value = value;
        this->is_return = true;
    }
    explicit ReturnStmt(int *location) : Stmt(location, "ReturnStmt") {
        this->value = nullptr;
        this->is_return = true;
    }
    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** String constants. */
class StringLiteral : public Literal {
public:
    /** Contents of the literal, not including quotation marks. */
    string value;
    /** The AST for a string literal containing VALUE, spanning source
     *  locations [LEFT..RIGHT]. */
    StringLiteral(int *location, const string &value);

    int *get_location() override { return this->location; }

    void accept(ast::Visitor &visitor) override;
};

/** An expression applying a unary operator. */
class UnaryExpr : public Expr {
public:
    enum class operator_code { Minus = 0, Not, Unknown };

    string operator_;
    Expr *operand;

    /** The AST for
     *      OPERATOR OPERAND
     *  spanning source locations [LEFT..RIGHT].
     */
    UnaryExpr(int *location, string operator_, Expr *operand) : Expr(location, "UnaryExpr") {
        this->operand = operand;
        this->operator_ = std::move(operator_);
    }

    static operator_code hashcode(std::string const &str) {
        if (str == "-" || str == "MINUS:-")
            return operator_code::Minus;
        if (str == "not" || str == "NOT:not")
            return operator_code::Not;
        return operator_code::Unknown;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** A declaration of a variable (i.e., with type annotation). */
class VarDef : public Decl {
public:
    /** The variable and its assigned type. */
    TypedVar *var;
    /** The initial value assigned. */
    Literal *value;
    /** The list is not used in function passing or class passing */
    bool is_conslist = false;

    /** The AST for
     *      VAR = VALUE
     *  where VAR has a type annotation, and spanning source
     *  locations [LEFT..RIGHT]. */
    VarDef(int *location, TypedVar *var, Literal *value) : Decl(location, "VarDef") {
        this->var = var;
        this->value = value;
    }

    Ident *get_id() override { return var->identifier; }
    int *get_location() override { return this->location; }
    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** Indefinite repetition construct. */
class WhileStmt : public Stmt {
public:
    /** Test for whether to continue. */
    Expr *condition;
    /** Loop body. */
    vector<Stmt *> *body;

    /** The AST for
     *      while CONDITION:
     *          BODY
     *  spanning source locations [LEFT..RIGHT].
     */
    WhileStmt(int *location, Expr *condition, vector<Stmt *> *body) : Stmt(location, "WhileStmt") {
        this->condition = condition;
        this->body = body;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;
    void accept(ast::Visitor &visitor) override;
};

/** Indefinite repetition construct. */
class PassStmt : public Stmt, public Decl {
public:
    /** The AST for
     *      PASS
     *  spanning source locations [LEFT..RIGHT].
     */
    explicit PassStmt(int *location) : Stmt(location, "PassStmt"), Decl(location, "PassStmt") {}
    void accept(ast::Visitor &visitor) override;
    cJSON *toJSON() override;
};

/** Var assignment expr. */
class VarAssignExpr : public Expr {
public:
    Ident *var;
    Expr *value;

    VarAssignExpr(int *location, Ident *var, Expr *value) : Expr(location, "VarAssignExpr") {
        this->var = var;
        this->value = value;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** Member assignment expr. */
class MemberAssignExpr : public Expr {
public:
    MemberExpr *objectMember;
    Expr *value;

    MemberAssignExpr(int *location, MemberExpr *objectMember, Expr *value) : Expr(location, "MemberAssignExpr") {
        this->objectMember = objectMember;
        this->value = value;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** Index assignment expr. */
class IndexAssignExpr : public Expr {
public:
    IndexExpr *listElement;
    Expr *value;

    IndexAssignExpr(int *location, IndexExpr *listElement, Expr *value) : Expr(location, "IndexAssignExpr") {
        this->listElement = listElement;
        this->value = value;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** Var assignment stmt. */
class VarAssignStmt : public Stmt {
public:
    Ident *var;
    Expr *value;
    bool is_len_list = false;

    VarAssignStmt(int *location, Ident *var, Expr *value) : Stmt(location, "VarAssignStmt") {
        this->var = var;
        this->value = value;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** Member assignment stmt. */
class MemberAssignStmt : public Stmt {
public:
    MemberExpr *objectMember;
    Expr *value;

    MemberAssignStmt(int *location, MemberExpr *objectMember, Expr *value) : Stmt(location, "MemberAssignStmt") {
        this->objectMember = objectMember;
        this->value = value;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

/** Index assignment stmt. */
class IndexAssignStmt : public Stmt {
public:
    IndexExpr *listElement;
    Expr *value;

    IndexAssignStmt(int *location, IndexExpr *listElement, Expr *value) : Stmt(location, "IndexAssignStmt") {
        this->listElement = listElement;
        this->value = value;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};

class AccOptions : public Node {
public:
    enum class AccType { VECTOR, WORKER };

    AccType accType;
    int num;
    /** The AST for
     *      #acc parallel loop worker/vector(x)
     *  spanning source locations [LEFT..RIGHT].
     */
    AccOptions(int *location, int type, int num) : Node(location, "AccOptions") {
        this->accType = (AccType)type;
        this->num = num;
    }

    int *get_location() override { return this->location; }

    cJSON *toJSON() override;

    void accept(ast::Visitor &visitor) override;
};
cJSON *add_inferred_type(semantic::SymbolType *class_);
} // namespace parser

#endif // CHOCOPY_COMPILER_PARSE_HPP