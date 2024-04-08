#include "../headers/AstPrinter.hpp"
#include "../headers/Stmt.hpp"
#include "../headers/Expr.hpp"

std::string AstPrinter::print(std::initializer_list<expr_ptr>& exprs) {
    for(const auto& expr : exprs) {
        expr.get()->accept(*this);
    }

    return oss.str();
}

std::string AstPrinter::parenthesize(std::string name, std::initializer_list<const Expr*> exprs) {

    oss << "(";
    oss << name;

    for(const auto& expr : exprs) {
        oss << " ";
        oss << std::any_cast<std::string>(expr->accept(*this));
    }
    oss << ")";

    return {};
}

Object AstPrinter::visitBinaryExpr(const Binary& expr) {
    return parenthesize(expr.operator_.lexeme, {std::move(expr.left.get()), std::move(expr.right.get())});
}

Object AstPrinter::visitGroupingExpr(const Grouping& expr) {
    return parenthesize("group", {std::move(expr.expression.get())});
}

Object AstPrinter::visitLiteralExpr(const Literal& expr) {
    if(std::visit(Token::Resolver{}, expr.value) == "null") return "nil";
    oss << std::visit(Token::Resolver{}, expr.value);

    return std::string{""};
}

Object AstPrinter::visitUnaryExpr(const Unary& expr) {
    return parenthesize(expr.operator_.lexeme, {std::move(expr.right.get())});
}