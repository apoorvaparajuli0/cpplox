#include "../headers/Token.hpp"

Token::Token(TokenType type, std::string &lexeme, Object &literal, int line) : 
            type(type), lexeme(lexeme), literal(literal), line(line)
{}

std::string Token::toString() {
    return typeNames[type] + " " + lexeme + " " + std::visit(Resolver{}, literal) + "\n";
}