//
// Created by yiwei yang on 2/15/21.
//
#include <chocopy_ast.hpp>

void parser::AssignStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::IfStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::PassStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::BinaryExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::BoolLiteral::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::CallExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::ClassDef::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::ClassType::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::ExprStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::ForStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::FuncDef::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::GlobalDecl::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::Ident::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::IfExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::IndexExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::IntegerLiteral::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::ListExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::ListType::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::MemberExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::MethodCallExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::NoneLiteral::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::NonlocalDecl::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::Program::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::ReturnStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::StringLiteral::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::TypedVar::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::UnaryExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::VarDef::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::WhileStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::VarAssignStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::MemberAssignStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::IndexAssignStmt::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::VarAssignExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::MemberAssignExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::IndexAssignExpr::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::AccOptions::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::Err::accept(ast::Visitor &visitor) { visitor.visit(*this); }
void parser::Node::accept(ast::Visitor &visitor) { visitor.visit(*this); }
