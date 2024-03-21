#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "vector"
#include "unordered_map"
#include "Token.hpp"

class Scanner {
    private:
        std::string source;
        std::vector<Token> tokens;
        int start = 0;
        int current = 0;
        int line = 1;
        
        //CHALLENGE #01: Add Support for Multi-Line Nested Comments
        // int commentLayer = 0;

        static std::unordered_map<std::string, TokenType> keywords;

        bool isAtEnd();
        void scanToken();
        char advance();
        void addToken(TokenType token);
        void addToken(TokenType token, Object& literal);
        bool match(char expected);
        char peek();
        void string();
        bool isDigit(char c);
        void number();
        char peekNext();
        void identifier();
        bool isAlpha(char c);
        bool isAlphanumeric(char c);

    public:
        Scanner(std::string &source);
    
    std::vector<Token> scanTokens();
};

#endif