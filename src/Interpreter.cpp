#include "../headers/Interpreter.hpp"
#include "utility"

std::any Interpreter::visitGroupingExpr(const Grouping& expr) {
    return evaluate(expr.expression);
}

std::any Interpreter::visitLiteralExpr(const Literal& expr) {
    return expr.value;
}

/**
 * This is a little hacky, we're gonna have to make sure to throw a runtime error if a non-double
 * operand is resolved from the variant prior to casting it to a double
*/
std::any Interpreter::visitUnaryExpr(const Unary& expr) {
    Object right = std::any_cast<Object>(evaluate(expr.right));

    switch(expr.operator_.type) {
        case MINUS:
            return -(std::any_cast<double>(std::visit(Token::ValueResolver{}, right)));
        case BANG:
            return !isTruthy(right);
    }

    return NULL;
}

bool Interpreter::isTruthy(Object object) {
    if(std::holds_alternative<std::nullptr_t>(object)) return false;
    if(std::holds_alternative<bool>(object)) return std::any_cast<bool>(std::visit(Token::ValueResolver{}, object));
    return true;
}

bool Interpreter::isEqual(Object a, Object b) {
    if((std::holds_alternative<std::nullptr_t>(a) && std::holds_alternative<std::nullptr_t>(b))) return true;
    if((std::holds_alternative<std::nullptr_t>(a))) return false;
    if(std::visit(Token::TypeResolver{}, a) != std::visit(Token::TypeResolver{}, b)) return false;

    auto equality = [](Object& op1, Object& op2) -> bool {
        if(const double* ptr = std::get_if<double>(&op1)) {
            return *ptr == *(std::get_if<double>(&op2));
        } else if(const bool* ptr = std::get_if<bool>(&op1)) {
            return *ptr == *(std::get_if<bool>(&op2));
        } else if(const std::string* ptr = std::get_if<std::string>(&op1)) {
            return *ptr == *(std::get_if<std::string>(&op2));
        }
        
        return false;
    };

    return equality(a, b);
}

std::any Interpreter::evaluate(const expr_ptr& expr) {
    return expr.get()->accept(*this);
}

std::any Interpreter::visitBinaryExpr(const Binary& expr) {
    Object left = std::any_cast<Object>(evaluate(expr.left));
    Object right = std::any_cast<Object>(evaluate(expr.right));

    std::any left_operand = std::visit(Token::ValueResolver{}, left);
    std::any right_operand = std::visit(Token::ValueResolver{}, right);

    switch(expr.operator_.type) {
        case GREATER:
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case GREATER_EQUAL:
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case LESS:
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case LESS_EQUAL:
            return std::any_cast<double>(left) <= std::any_cast<double>(right);

        case BANG_EQUAL: return !isEqual(left, right);
        case EQUAL_EQUAL: return isEqual(left, right);

        case MINUS:
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case PLUS:
            if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            } else if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }
            break;
        case SLASH:
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case STAR:
            return std::any_cast<double>(left) * std::any_cast<double>(right);
    }

    return NULL;
}