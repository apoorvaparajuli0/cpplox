#include "../headers/Lox.hpp"
#include "../headers/AstPrinter.hpp"
#include "../headers/RPNPrinter.hpp"

int main(int argc, char** argv){
    if(argc > 2) {
        printf("%s", "Usage: cpplox [script]");
        exit(64);
    } else if(argc == 2) {
        Lox::runFile(argv[1]);
    } else {
        Lox::runPrompt();
    }

    return 0;
}
