#ifndef LOX_HPP
#define LOX_HPP

#include "string"
#include "Token.hpp"

class Lox {
    static bool hadError;
    
    public:
        static void runFile(std::string path);
        static void runPrompt();
        static void error(int line, std::string message);
        static void error(Token token, std::string message);

    private:
        static void run(std::string src);
        static void report(int line, std::string where, std::string message);
};

#endif