#include "../headers/Stmt.hpp"

Block::Block(std::vector<stmt_ptr> statements) : 
statements{std::move(statements)} {}
std::any Block::accept(StmtVisitor<std::any>& visitor) const {
  return visitor.visitBlockStmt(*this);
}

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

