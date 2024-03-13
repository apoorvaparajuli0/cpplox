#include "string"

class Lox {
    static bool hadError;

    static void error(int line, std::string message);
    
    public:
        static void runFile(std::string path);
        static void runPrompt();

    private:
        static void run(std::string src);
        static void report(int line, std::string where, std::string message);
};