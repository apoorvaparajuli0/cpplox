#ifndef PARSER_HPP
#define PARSER_HPP

#include "vector"
#include "TokenType.hpp"
#include "Token.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

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
        /**
         * CHALLENGE(S) 6.1 & 6.2:
         * Add Support for C-Style Ternary Operations
         * and the Comma operator
        */
        // expr_ptr ternary();
        // expr_ptr branch();
        // expr_ptr comma();
        expr_ptr expression();
        stmt_ptr declaration();
        stmt_ptr statement();
        stmt_ptr printStatement();
        stmt_ptr varDeclaration();
        stmt_ptr expressionStatement();
        std::vector<stmt_ptr> block();
        expr_ptr assignment();
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
        expr_ptr parseBinaryExpression(std::initializer_list<TokenType> tokenTypes, expr_ptr (Parser::* l_operand)(), expr_ptr (Parser::* r_operand)());

};

#endif