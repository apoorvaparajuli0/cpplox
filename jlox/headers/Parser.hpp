#ifndef PARSER_HPP
#define PARSER_HPP

#include "stdexcept"
#include "vector"
#include "memory"
#include "initializer_list"
#include "string"

#include "Token.hpp"
#include "Visitor.hpp"
#include "TokenType.hpp"

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

        std::vector<stmt_ptr> parse();
    
    private:
        stmt_ptr declaration();
        stmt_ptr classDeclaration();
        stmt_ptr statement();
        stmt_ptr forStatement();
        stmt_ptr ifStatement();
        stmt_ptr printStatement();
        stmt_ptr returnStatement();
        stmt_ptr varDeclaration();
        stmt_ptr whileStatement();
        stmt_ptr expressionStatement();
        stmt_ptr function(std::string kind);
        std::vector<stmt_ptr> block();

        expr_ptr expression();
        expr_ptr assignment();
        expr_ptr log_or();
        expr_ptr log_and();
        expr_ptr equality();
        expr_ptr comparison();
        expr_ptr term();
        expr_ptr factor();
        expr_ptr unary();
        expr_ptr finishCall(expr_ptr& callee);
        expr_ptr call();
        expr_ptr primary();

        bool match(std::initializer_list<TokenType> types);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        Token consume(TokenType type, std::string message);
        expr_ptr parseBinaryExpression(std::initializer_list<TokenType> tokenTypes, expr_ptr (Parser::* l_operand)(), expr_ptr (Parser::* r_operand)());

        ParseError error(Token token, std::string message);
        void synchronize();

};

#endif