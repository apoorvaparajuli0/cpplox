#include "any"
#include "memory"
#include "Token.hpp"

class Binary;
class Grouping;
class Literal;
class Unary;

template<class R>
class Visitor {
  public:
    virtual R visitBinaryExpr(const Binary& expr) = 0;
    virtual R visitGroupingExpr(const Grouping& expr) = 0;
    virtual R visitLiteralExpr(const Literal& expr) = 0;
    virtual R visitUnaryExpr(const Unary& expr) = 0;
    virtual ~Visitor() = default;
};

class Expr {
  public:
    virtual std::any accept(Visitor<std::any>& visitor) const = 0;
    virtual ~Expr() = default;
};

using expr_ptr = std::unique_ptr<Expr>;
