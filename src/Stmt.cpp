#include "../headers/Stmt.hpp"

    Expression::Expression(Expr expression) : 
    expression{std::move(expression)} {}
    std::any Expression::accept(Visitor<std::any>& visitor) const {
      return visitor.visitExpressionStmt(*this);
    }

    Print::Print(Expr expression) : 
    expression{std::move(expression)} {}
    std::any Print::accept(Visitor<std::any>& visitor) const {
      return visitor.visitPrintStmt(*this);
    }

