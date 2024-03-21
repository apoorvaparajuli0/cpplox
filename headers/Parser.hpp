#ifndef PARSER_HPP
#define PARSER_HPP

#include "vector"
#include "TokenType.hpp"
#include "Token.hpp"
#include "Expr.hpp"

class Parser {

    std::vector<Token> tokens;
    int current = 0;

    public:
        class ParseError : std::runtime_error {
            public:
                ParseError() : runtime_error("Parser Error") {}
        };

        Parser(std::vector<Token> tokens) {
            this->tokens = tokens;
        };
        expr_ptr parse();
    
    private:
        expr_ptr ternary();
        expr_ptr branch();
        expr_ptr comma();
        expr_ptr expression();
        expr_ptr equality();
        bool match(std::initializer_list<TokenType> types);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        expr_ptr comparison();
        expr_ptr term();
        expr_ptr factor();
        expr_ptr unary();
        expr_ptr primary();
        Token consume(TokenType type, std::string message);
        ParseError error(Token token, std::string message);
        void synchronize();
};

#endif