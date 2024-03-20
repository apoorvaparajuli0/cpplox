#ifndef LOX_HPP
#define LOX_HPP

#include "string"

class Lox {
    static bool hadError;
    
    public:
        static void runFile(std::string path);
        static void runPrompt();
        static void error(int line, std::string message);

    private:
        static void run(std::string src);
        static void report(int line, std::string where, std::string message);
};

#endif