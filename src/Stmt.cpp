#include "../headers/Stmt.hpp"

Expression::Expression(expr_ptr expression) : 
expression{std::move(expression)} {}
std::any Expression::accept(StmtVisitor<std::any>& visitor) const {
  return visitor.visitExpressionStmt(*this);
}

Print::Print(expr_ptr expression) : 
expression{std::move(expression)} {}
std::any Print::accept(StmtVisitor<std::any>& visitor) const {
  return visitor.visitPrintStmt(*this);
}

Var::Var(Token name, expr_ptr initializer) : 
name{std::move(name)}, initializer{std::move(initializer)} {}
std::any Var::accept(StmtVisitor<std::any>& visitor) const {
  return visitor.visitVarStmt(*this);
}

