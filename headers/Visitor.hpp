#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "any"
#include "memory"
#include "../headers/Token.hpp"

class Binary;
class Grouping;
class Literal;
class Unary;


class Expression;
class Print;


template<class R>
class ExprVisitor {
  public:
    virtual R visitBinaryExpr(const Binary& expr) = 0;
    virtual R visitGroupingExpr(const Grouping& expr) = 0;
    virtual R visitLiteralExpr(const Literal& expr) = 0;
    virtual R visitUnaryExpr(const Unary& expr) = 0;
    virtual ~ExprVisitor() = default;
};

template<class R>
class StmtVisitor {
  public:
    virtual R visitExpressionStmt(const Expression& stmt) = 0;
    virtual R visitPrintStmt(const Print& stmt) = 0;
    virtual ~StmtVisitor() = default;
};

class Expr {
  public:
    virtual Object accept(ExprVisitor<Object>& visitor) const = 0;
    virtual ~Expr() = default;
};

class Stmt {
  public:
    virtual std::any accept(StmtVisitor<std::any>& visitor) const = 0;
    virtual ~Stmt() = default;
};

using expr_ptr = std::unique_ptr<Expr>;
using stmt_ptr = std::unique_ptr<Stmt>;

#endif
