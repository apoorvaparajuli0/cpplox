#include "list"
#include "any"
#include "Token.hpp"


class Expr {
  public:
    class Binary;
    class Grouping;
    class Literal;
    class Unary;

    virtual std::any accept(Visitor visitor);
};

class Visitor {
  public:
    virtual std::any visitBinaryExpr(Expr::Binary expr);
    virtual std::any visitGroupingExpr(Expr::Grouping expr);
    virtual std::any visitLiteralExpr(Expr::Literal expr);
    virtual std::any visitUnaryExpr(Expr::Unary expr);
};
