//
// Created by yiwei yang on 2/16/21.
//

#ifndef CHOCOPY_COMPILER_AST_HPP
#define CHOCOPY_COMPILER_AST_HPP

#pragma once

#include <chocopy_parse.hpp>
#include <iostream>
#include <vector>

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

parser::Program *parse(const char *input_path);

namespace ast {
class Visitor {
public:
    virtual void visit(parser::AssignStmt &) = 0;
    virtual void visit(parser::Program &) = 0;
    virtual void visit(parser::PassStmt &) = 0;
    virtual void visit(parser::BinaryExpr &) = 0;
    virtual void visit(parser::BoolLiteral &) = 0;
    virtual void visit(parser::CallExpr &) = 0;
    virtual void visit(parser::ClassDef &) = 0;
    virtual void visit(parser::ClassType &) = 0;
    virtual void visit(parser::ExprStmt &) = 0;
    virtual void visit(parser::ForStmt &) = 0;
    virtual void visit(parser::FuncDef &) = 0;
    virtual void visit(parser::GlobalDecl &) = 0;
    virtual void visit(parser::Ident &) = 0;
    virtual void visit(parser::IfExpr &) = 0;
    virtual void visit(parser::IndexExpr &) = 0;
    virtual void visit(parser::IntegerLiteral &) = 0;
    virtual void visit(parser::ListExpr &) = 0;
    virtual void visit(parser::ListType &) = 0;
    virtual void visit(parser::MemberExpr &) = 0;
    virtual void visit(parser::IfStmt &node) = 0;
    virtual void visit(parser::MethodCallExpr &) = 0;
    virtual void visit(parser::NoneLiteral &) = 0;
    virtual void visit(parser::NonlocalDecl &) = 0;
    virtual void visit(parser::ReturnStmt &) = 0;
    virtual void visit(parser::StringLiteral &) = 0;
    virtual void visit(parser::TypeAnnotation &) = 0;
    virtual void visit(parser::TypedVar &) = 0;
    virtual void visit(parser::UnaryExpr &) = 0;
    virtual void visit(parser::VarDef &) = 0;
    virtual void visit(parser::WhileStmt &) = 0;
    virtual void visit(parser::VarAssignStmt &) = 0;
    virtual void visit(parser::MemberAssignStmt &) = 0;
    virtual void visit(parser::IndexAssignStmt &) = 0;
    virtual void visit(parser::VarAssignExpr &) = 0;
    virtual void visit(parser::MemberAssignExpr &) = 0;
    virtual void visit(parser::IndexAssignExpr &) = 0;
    virtual void visit(parser::Err &) = 0;
    virtual void visit(parser::Node &) = 0;
};

} // namespace ast

#endif // CHOCOPY_COMPILER_AST_HPP
