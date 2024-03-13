#include "TokenType.hpp"
#include "Token.hpp"

Token::Token(TokenType type, std::string &lexeme, Object &literal, int line) : 
            type(type), lexeme(lexeme), literal(literal), line(line)
{}

std::string Token::toString() {
    return typeNames[Token::type] + " " + Token::lexeme + std::visit(Resolver{}, Token::literal);
}